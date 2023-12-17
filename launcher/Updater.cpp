#include "Updater.h"
#include "document.h"

#include <curl/curl.h>
#include <string>

std::string buffer;
std::string version = "dev build";

using namespace rapidjson;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void CheckForUpdates() {
    printf("Checking for updates\n");


    CURL* curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        std::string result;
        rapidjson::Document doc;

        // Check the GitHub API for a new release. The REST API automatically filters out prereleases so these aren't a concern.
        struct curl_slist *headers = NULL;

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

        /* always cleanup */
        curl_easy_cleanup(curl);

        doc.Parse(result.c_str());
        printf("%s\n", doc["name"].GetString());
    }

    printf("Finished\n");
}

