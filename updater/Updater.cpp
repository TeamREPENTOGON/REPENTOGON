#include <Windows.h>

#include "dsound/Logger.h"
#include "document.h"
#include "dsound/utils.h"
#include "updater/updater.h"

#include <curl/curl.h>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>

#include <cryptopp/sha.h>
#include <cryptopp/hex.h>

#define MINI_CASE_SENSITIVE
#include "ini.h"

std::string version = "dev build";

using namespace rapidjson;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    sLogger->Debug("dsound: curl_easy_perform: WriteCallback: size = %lu, nmemb = %lu\n", size, nmemb);
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string trim(const std::string& input) {
    std::stringstream string_stream;
    for (const auto character : input) {
        if (!isspace(character)) {
            string_stream << character;
        }
    }

    return string_stream.str();
}

mINI::INIStructure ini;

mINI::INIFile iniFile("dsound.ini");

int main() {
    sLogger->SetLogLevel(LOG_INFO);
    sLogger->SetOutputFile("repentogon_updater.log", "w", true);
    sLogger->SynchronizeWithStdout(true);
    sLogger->Info("Started base updater\n");
    bool ok = true;
    if (!std::filesystem::exists("dsound.ini")) {
        sLogger->Info("dsound.ini doesn't exist, creating\n");
        if (MessageBoxA(NULL, "Would you like REPENTOGON to automatically check for updates on game start?\n(We highly recommend saying yes here, we're probably gonna have a lot of them.)", "REPENTOGON", MB_YESNO | MB_SETFOREGROUND | MB_TOPMOST | MB_TASKMODAL) == IDYES)
            ini["Options"]["CheckForUpdates"] = "1";
        else
            ini["Options"]["CheckForUpdates"] = "0";

        ini["internal"]["RanUpdater"] = "0";
        ok = iniFile.generate(ini, true);
    }

    if (!ok) {
        sLogger->Error("dsound: CheckForUpdates: Error while generating initial dsound.ini file, skipping\n");
        return UPDATER_EXIT_ERROR;
    }

    ok = iniFile.read(ini);
    if (!ok) {
        sLogger->Error("dsound: CheckForUpdates: Error while reading dsound.ini, skipping\n");
        return UPDATER_EXIT_ERROR;
    }

    if (ini["Options"]["CheckForUpdates"] == "0")
        return UPDATER_EXIT_SKIPPED;

    if (ini["internal"]["RanUpdater"] == "1") {
        ini["internal"]["RanUpdater"] = "0";
        iniFile.write(ini);
        return UPDATER_EXIT_CLEANUP;
    }

    if (version.compare("dev build") == 0 || version.find("nightly-") != std::string::npos) {
        sLogger->Info("Skipping update check (dev build)\n");
        return UPDATER_EXIT_DEV_VERSION;
    }
    else {
        sLogger->Info("Checking for updates\n");

        CURL* curl;
        CURLcode res;

        curl = curl_easy_init();
        if (curl) {
            std::string result;
            rapidjson::Document doc;
            rapidjson::Document updater_doc;

            // Check the GitHub API for a new release. The REST API automatically filters out prereleases so these aren't a concern.
            struct curl_slist* headers = NULL;

            headers = curl_slist_append(headers, "Accept: application/vnd.github+json");
            headers = curl_slist_append(headers, "X-GitHub-Api-Version: 2022-11-28");
            headers = curl_slist_append(headers, "User-Agent: REPENTOGON");

            curl_easy_setopt(curl, CURLOPT_URL, "https://api.github.com/repos/TeamREPENTOGON/REPENTOGON/releases/latest");
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            curl_easy_setopt(curl, CURLOPT_SERVER_RESPONSE_TIMEOUT, 5);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);

            sLogger->Info("dsound: CheckForUpdates: curl_easy_perform\n");
            res = curl_easy_perform(curl);
            sLogger->Info("dsound: CheckForUpdates: curl_easy_perform done\n");
            if (res != CURLE_OK) {
                const char* strError = curl_easy_strerror(res);
                sLogger->Error("dsound: CheckForUpdates: Failed to check for updates: %s\n", strError);
                return UPDATER_EXIT_ERROR;
            }

            doc.Parse(result.c_str());

            if (!doc.HasMember("name")) {
                const char* returnedJson = result.c_str();
                sLogger->Error("dsound: CheckForUpdates: Failed to fetch latest release from the GitHub API. Returned JSON: %s\n", returnedJson);
                return UPDATER_EXIT_ERROR;
            }
            // If the release name is different from our version name, we are out of date!
            else if(version.compare(doc["name"].GetString()) != 0) {
                char message[128];
                sprintf(message, "A new version of REPENTOGON is available!\nCurrent version: %s\nNew version: %s\n\nUpdate now?", version.c_str(), doc["name"].GetString());
                if (MessageBoxA(NULL, message, "REPENTOGON", MB_YESNO) == IDYES) {
                    result.clear();
                    curl_easy_setopt(curl, CURLOPT_URL, "https://api.github.com/repos/TeamREPENTOGON/Installer/releases/latest");
                    res = curl_easy_perform(curl);
                    if (res != CURLE_OK) {
                        const char* err = curl_easy_strerror(res);
                        sLogger->Error("dsound: CheckForUpdates: Failed to check for updates: %s\n", err);
                        return UPDATER_EXIT_ERROR;
                    }

                    updater_doc.Parse(result.c_str());
                    if (!updater_doc.HasMember("name")) {
                        sLogger->Error("dsound: CheckForUpdates: Failed to fetch latest release from GitHub. Returned JSON: %s\n", result.c_str());
                        curl_easy_cleanup(curl);
                        return UPDATER_EXIT_ERROR;
                    }

                    std::string updater_hash;
                    std::string hash_url;
                    std::string updater_url;
                    std::string exe_content;
                    std::string calculated_hash;
                    CryptoPP::SHA256 sha256;

                    for (const auto& asset : updater_doc["assets"].GetArray()) {
                        if (std::string("hash.txt").compare(asset["name"].GetString()) == 0)
                            hash_url = asset["browser_download_url"].GetString();

                        if (std::string("REPENTOGONUpdater.exe").compare(asset["name"].GetString()) == 0)
                            updater_url = asset["browser_download_url"].GetString();
                    }

                    if (hash_url.empty() || updater_url.empty()) {
                        sLogger->Error("dsound: CheckForUpdates: Unable to fetch URL of updater or hash, update cannot continue.\nHash URL: %s\nUpdater URL: %s", hash_url.c_str(), updater_url.c_str());
                        curl_easy_cleanup(curl);
                        return UPDATER_EXIT_ERROR;
                    }

                    sLogger->Info("dsound: CheckForUpdates: Fetching hash from %s\n", hash_url.c_str());
                    curl_easy_setopt(curl, CURLOPT_URL, hash_url.c_str());
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &updater_hash);

                    res = curl_easy_perform(curl);
                    if (res != CURLE_OK) {
                        sLogger->Error("Failed to download hash: %s\n",
                            curl_easy_strerror(res));
                        curl_easy_cleanup(curl);
                        return UPDATER_EXIT_ERROR;
                    }

                    sLogger->Info("dsound: CheckForUpdates: Downloading updater\n");
                    FILE* exe = fopen("REPENTOGONUpdater.exe", "wb");
                    curl_easy_setopt(curl, CURLOPT_URL, updater_url.c_str());
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &exe_content);

                    res = curl_easy_perform(curl);
                    if (res != CURLE_OK) {
                        sLogger->Error("Failed to download release: %s\n",
                            curl_easy_strerror(res));
                        curl_easy_cleanup(curl);
                        return UPDATER_EXIT_ERROR;
                    }
                    fclose(exe);
                    curl_easy_cleanup(curl);

                    CryptoPP::StringSource(exe_content, true,
                        new CryptoPP::HashFilter(sha256,
                            new CryptoPP::HexEncoder(
                                new CryptoPP::StringSink(calculated_hash)
                            )
                        )
                    );

                    std::transform(calculated_hash.begin(), calculated_hash.end(), calculated_hash.begin(),
                        [](unsigned char c) { return std::tolower(c); });

                    std::transform(updater_hash.begin(), updater_hash.end(), updater_hash.begin(),
                        [](unsigned char c) { return std::tolower(c); });

                    updater_hash = trim(updater_hash);

                    sLogger->Debug("Hash: %s\n", updater_hash.c_str());

                    if (updater_hash.compare(calculated_hash) != 0) {
                        sLogger->Error("Hash mismatch, aborting!\nExpected hash: %s\nCalculated hash: %s\n", updater_hash.c_str(), calculated_hash.c_str());
                        curl_easy_cleanup(curl);
                        return UPDATER_EXIT_ERROR;
                    }

                    std::ofstream updater("REPENTOGONUpdater.exe", std::ios::out | std::ios::binary);
                    if (!updater.good()) {
                        sLogger->Error("Unable to open file to write the updater, aborting\n");
                        curl_easy_cleanup(curl);
                        return UPDATER_EXIT_ERROR;
                    }

                    updater.write(exe_content.c_str(), exe_content.size());
                    updater.close();

                    curl_easy_cleanup(curl);
                    sLogger->Info("dsound: CheckForUpdates: Finished downloading, launching updater\n");
                    // chnage da world,
                    ini["internal"]["RanUpdater"] = "1";
                    iniFile.write(ini);

                    if (!argv)
                        argv = CommandLineToArgvA(GetCommandLineA(), &argc);

                    std::string args = "-auto";
                    for (int i = 1; i < argc; ++i) {
                        args.append(" ").append(argv[i]);
                    }

                    ShellExecute(NULL, "open", "REPENTOGONUpdater.exe", args.c_str(), NULL, SW_SHOWDEFAULT);
                    return UPDATER_EXIT_UPDATED;
                }
                else {
                    sLogger->Info("Skipping update as per user request\n");
                    return UPDATER_EXIT_DONT_UPDATE;
                }
            }
        }
        else {
            sLogger->Error("Unable to initialize cURL\n");
            return UPDATER_EXIT_ERROR;
        }
    }

    return UPDATER_EXIT_NO_UPDATE_AVAILABLE;
}

