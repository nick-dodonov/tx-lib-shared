#include "Boot/Boot.h"
#include "Log/Log.h"
#include <iostream>
#include <emscripten/fetch.h>

/// HTTP response structure
struct HttpResponse {
    int status_code = 0;
    std::string headers;
    std::string body;
};

/// Make HTTP or HTTPS request to a server
HttpResponse makeHttpRequest(const std::string& host, int port, const std::string& path, bool use_tls = false)
{
    HttpResponse response;

    //TODO: Implement the HTTP request logic here using emscripten fetch

    /** SAMPLE:
    void onSuccess(emscripten_fetch_t *fetch) {
        printf("Downloaded %llu bytes\n", fetch->numBytes);
        // Обработка fetch->data
        emscripten_fetch_close(fetch);
    }

    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = onSuccess;
    
    // Браузер автоматически использует HTTP/2 или HTTP/3
    emscripten_fetch(&attr, "https://example.com/api");
    **/

    return response;
}

int main(int argc, char** argv)
{
    Boot::LogInfo(argc, argv);
    Log::DefaultInit();

    // Request 1: Plain HTTP to ifconfig.io
    Log::Info("========== Request 1: HTTP ifconfig.io ==========");
    HttpResponse resp1 = makeHttpRequest("ifconfig.io", 80, "/", false);
    Log::InfoF("===== Status Code: {}", resp1.status_code);
    Log::Info("===== Headers:");
    std::cout << resp1.headers << "\n";
    Log::Info("===== Body:");
    std::cout << resp1.body << "\n";

    // Request 2: HTTPS to httpbin.org
    Log::Info("========== Request 2: HTTPS httpbin.org /headers ==========");
    HttpResponse resp2 = makeHttpRequest("httpbin.org", 443, "/headers", true);
    Log::InfoF("===== Status Code: {}", resp2.status_code);
    Log::Info("===== Headers:");
    std::cout << resp2.headers << "\n";
    Log::Info("===== Body:");
    std::cout << resp2.body << "\n";

    return 0;
}
