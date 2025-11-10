#include "Boot/Boot.h"
#include "Log/Log.h"

#include <emscripten/emscripten.h>
#include <emscripten/fetch.h>
#include <iostream>
#include <queue>

/// HTTP response structure
struct HttpResponse {
    int status_code = 0;
    std::string headers;
    std::string body;
    bool completed = false;
};

/// Global state for request queue
static std::queue<HttpResponse> g_responses;
static int g_request_id = 0;
static int g_pending_requests = 0;

/// Callback for successful fetch
void onSuccess(emscripten_fetch_t* fetch)
{
    Log::InfoF("✓ Request completed with status: {}", fetch->status);

    HttpResponse response;
    response.status_code = fetch->status;
    response.body = std::string((char*)fetch->data, fetch->numBytes);
    response.headers = "Status: " + std::to_string(fetch->status) + " OK";
    response.completed = true;

    g_responses.push(response);
    g_pending_requests--;

    emscripten_fetch_close(fetch);
}

/// Callback for failed fetch
void onError(emscripten_fetch_t* fetch)
{
    Log::ErrorF("✗ Request failed with status: {}", fetch->status);

    HttpResponse response;
    response.status_code = fetch->status;
    response.body = "Request failed";
    response.headers = "Error: HTTP " + std::to_string(fetch->status);
    response.completed = true;

    g_responses.push(response);
    g_pending_requests--;

    emscripten_fetch_close(fetch);
}

/// Make HTTP or HTTPS request to a server
void makeHttpRequest(const std::string& url)
{
    g_request_id++;
    g_pending_requests++;

    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = onSuccess;
    attr.onerror = onError;
    attr.timeoutMSecs = 30000; // 30 second timeout

    Log::InfoF("Initiating fetch request #{} to: {}", g_request_id, url);
    emscripten_fetch(&attr, url.c_str());
}

/// Update function called each frame from main loop
void updateFrame()
{
    // Process all completed responses
    while (!g_responses.empty()) {
        HttpResponse resp = g_responses.front();
        g_responses.pop();

        Log::InfoF("Response received:");
        Log::InfoF("  Status: {}", resp.status_code);
        Log::InfoF("  Headers: {}", resp.headers);
        Log::InfoF("  Body length: {}", resp.body.length());

        // Print first 200 chars of body
        if (resp.body.length() > 0) {
            // std::string preview = resp.body.substr(0, std::min(size_t(200), resp.body.length()));
            // Log::InfoF("  Body preview: {}", preview);
            Log::Info("  Body preview:");
            std::cout << resp.body << "\n";
        }
    }

    // Stop main loop when all requests are done
    if (g_pending_requests == 0 && g_request_id > 0) {
        Log::Info("");
        Log::Info("All requests completed. Exiting.");

        // Check if running in browser or Node.js via JS module flag
        bool is_browser =
            EM_ASM_INT({ 
                return (typeof Module !== 'undefined' && Module.__EMSCRIPTEN_IS_BROWSER__) ? 1 : 0;
            });

        if (is_browser) {
            // Browser environment - cancel main loop gracefully
            emscripten_cancel_main_loop();
        }
        else {
            // Node.js or other environment - exit directly
            exit(0);
        }
    }
}

int main(int argc, char** argv)
{
    Boot::LogInfo(argc, argv);
    Log::DefaultInit();

    Log::Info("========== Emscripten Fetch API Example ==========");
    Log::Info("This example demonstrates async HTTP requests with callbacks");
    Log::Info("");

    // Make async request
    Log::Info("Request 1: Making async request to example.com");
    makeHttpRequest("http://example.com/");
    // makeHttpRequest("http://ifconfig.io/");

    Log::Info("Request initiated, waiting for response...");
    Log::Info("");

    // Setup and run main loop
    // The updateFrame() will be called repeatedly until all requests complete
    emscripten_set_main_loop(updateFrame, 0, true);

    return 0;
}
