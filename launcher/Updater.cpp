#include "Updater.h"
#include "document.h"
#include "utils.h"

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

void CheckForUpdates() {

    if (!std::filesystem::exists("dsound.ini")) {
        printf("dsound.ini doesn't exist, creating\n");
        if (MessageBoxA(NULL, "Would you like REPENTOGON to automatically check for updates on game start?\n(We highly recommend saying yes here, we're probably gonna have a lot of them.)", "REPENTOGON", MB_YESNO) == IDYES)
            ini["Options"]["CheckForUpdates"] = "1";
        else
            ini["Options"]["CheckForUpdates"] = "0";

        ini["internal"]["RanUpdater"] = "0";
        iniFile.generate(ini, true);
    }

    iniFile.read(ini);

    if (ini["Options"]["CheckForUpdates"] == "0")
        return;

    if (ini["internal"]["RanUpdater"] == "1") {
        ini["internal"]["RanUpdater"] = "0";
        iniFile.write(ini);
        return;
    }

    if (version.compare("dev build") == 0 || version.find("nightly-") != std::string::npos) {
        printf("Skipping update check (dev build)\n");
    }
    else {
        printf("Checking for updates\n");

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
            res = curl_easy_perform(curl);
            if (res != CURLE_OK)
                fprintf(stderr, "Failed to check for updates: %s\n",
                    curl_easy_strerror(res));

            doc.Parse(result.c_str());

            if (!doc.HasMember("name")) {
                fprintf(stderr, "Failed to fetch latest release from the GitHub API. Returned JSON: %s\n", result.c_str());
            }

            // If the release name is different from our version name, we are out of date!
            else if(version.compare(doc["name"].GetString()) != 0) {
                char message[128];
                sprintf(message, "A new version of REPENTOGON is available!\nCurrent version: %s\nNew version: %s\n\nUpdate now?", version.c_str(), doc["name"].GetString());
                if (MessageBoxA(NULL, message, "REPENTOGON", MB_YESNO) == IDYES) {

                    result.clear();
                    curl_easy_setopt(curl, CURLOPT_URL, "https://api.github.com/repos/TeamREPENTOGON/Installer/releases/latest");
                    res = curl_easy_perform(curl);
                    if (res != CURLE_OK)
                        fprintf(stderr, "Failed to check for updates: %s\n",
                            curl_easy_strerror(res));

                    updater_doc.Parse(result.c_str());
                    if (!updater_doc.HasMember("name")) {
                        fprintf(stderr, "Failed to fetch latest release from the GitHub API. Returned JSON: %s\n", result.c_str());
                        curl_easy_cleanup(curl);
                        return;
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
                        printf("Unable to fetch URL of updater or hash, update cannot continue.\nHash URL: %s\nUpdater URL: %s", hash_url.c_str(), updater_url.c_str());
                        curl_easy_cleanup(curl);
                        return;
                    }

                    printf("Fetching hash from %s...\n", hash_url.c_str());
                    curl_easy_setopt(curl, CURLOPT_URL, hash_url.c_str());
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &updater_hash);

                    res = curl_easy_perform(curl);
                    if (res != CURLE_OK) {
                        printf("Failed to download hash: %s\n",
                            curl_easy_strerror(res));
                        curl_easy_cleanup(curl);
                        return;
                    }

                    printf("Downloading updater...\n");
                    FILE* exe = fopen("REPENTOGONUpdater.exe", "wb");
                    curl_easy_setopt(curl, CURLOPT_URL, updater_url.c_str());
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &exe_content);

                    res = curl_easy_perform(curl);
                    if (res != CURLE_OK) {
                        printf("Failed to download release: %s\n",
                            curl_easy_strerror(res));
                        curl_easy_cleanup(curl);
                        return;
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

                    printf("Hash: %s\n", updater_hash.c_str());

                    if (updater_hash.compare(calculated_hash) != 0) {
                        printf("Hash mismatch, aborting!\nExpected hash: %s\nCalculated hash: %s\n", updater_hash.c_str(), calculated_hash.c_str());
                        curl_easy_cleanup(curl);
                        return;
                    }

                    std::ofstream updater("REPENTOGONUpdater.exe", std::ios::out | std::ios::binary);
                    if (updater.good()) {
                        updater.write(exe_content.c_str(), exe_content.size());
                        updater.close();
                    }
                    else {
                        printf("Unable to write to file, aborting\n");
                        curl_easy_cleanup(curl);
                        return;
                    }

                    curl_easy_cleanup(curl);
                    printf("Finished, launching updater\n");
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
                    ExitProcess(1);
                }
            }
        }
    }
}

