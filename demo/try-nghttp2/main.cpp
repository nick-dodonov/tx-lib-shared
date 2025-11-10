#include "Boot/Boot.h"
#include "Log/Log.h"
#include <nghttp2/nghttp2.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <iostream>

int main(int argc, char **argv) {
    Boot::LogInfo(argc, argv);
    Log::DefaultInit();

    nghttp2_info *info = nghttp2_version(0);
    Log::InfoF("nghttp2 version: {}", info->version_str);

    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        Log::Error("Failed to create socket");
        return 1;
    }

    // Resolve hostname
    struct hostent *server = gethostbyname("ifconfig.io");
    if (server == nullptr) {
        Log::Error("Failed to resolve hostname");
        close(sockfd);
        return 1;
    }

    // Connect to server
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(80);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    if (connect(sockfd, reinterpret_cast<struct sockaddr *>(&serv_addr),
                sizeof(serv_addr)) < 0) {
        Log::Error("Failed to connect to server");
        close(sockfd);
        return 1;
    }

    Log::Info("Connected to ifconfig.io:80");

    // Send HTTP/1.1 request
    std::string request = "GET / HTTP/1.1\r\n"
                          "Host: ifconfig.io\r\n"
                          "Connection: close\r\n"
                          "User-Agent: curl/8.7.1\r\n"
                          "\r\n";

    Log::InfoF("Sending HTTP request ({} bytes)...", request.size());
    if (send(sockfd, request.c_str(), request.size(), 0) < 0) {
        Log::Error("Failed to send request");
        close(sockfd);
        return 1;
    }

    // Receive response
    std::string response;
    char buffer[4096];
    ssize_t nread;
    while ((nread = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
        response.append(buffer, nread);
    }

    close(sockfd);

    // Output results
    Log::Info("\n========== HTTP Response ==========");
    std::cout.write(response.c_str(), response.size());
    std::cout << "\n===================================\n";

    return 0;
}
