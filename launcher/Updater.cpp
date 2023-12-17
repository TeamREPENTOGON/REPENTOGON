#include "Updater.h"
#include <curl/curl.h>
#include <thread>

void UpdaterThread() {
    printf("Checking for updates\n");
    CURL* curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");
        /* example.com is redirected, so we tell libcurl to follow redirection */
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    printf("Finished\n");
}



//If this thread is *joined* it will never start.
//If this thread is *detached* it works and runs alongside the game bootup.
void CheckForUpdates() {;
    std::thread(UpdaterThread).join();
}

