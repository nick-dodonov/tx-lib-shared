#include "Boot/Boot.h"
#include "Log/Log.h"
#include <asio.hpp>
#include <array>
#include <ctime>

using asio::ip::tcp;

static std::string make_daytime_string() {
    std::time_t now = time(0);
    return std::ctime(&now);
}

int main_server()
{
    asio::io_context io_context;

    Log::Info("initializing");
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 13));

    for (;;) {
        Log::Info("listening");

        tcp::socket socket(io_context);
        acceptor.accept(socket);

        Log::Info("accepted");
        std::string message = make_daytime_string();

        Log::Info(std::format("writing: {}", message));
        std::error_code ignored_error;
        asio::write(socket, asio::buffer(message), ignored_error);
    }
    return 0;
}

int main(int argc, char** argv)
{
    Boot::LogInfo(argc, argv);

    // try {
        if (argc != 2) {
            Log::Error(std::format("Usage: {} {{-s|<host>}}", argv[0]));
            Log::Error("Service emulation: `while true; do echo -ne \"$(date -u)\\0\" | nc -l 13; echo accessed; done`");
            return 1;
        }

        auto arg = argv[1];
        if (strcmp(arg, "-s") == 0) {
            return main_server();
        }

        asio::io_context io_context;
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve(argv[1], "daytime");
        tcp::socket socket(io_context);
        asio::connect(socket, endpoints);
        for (;;) {
            std::array<char, 128> buf;
            std::error_code error;

            size_t len = socket.read_some(asio::buffer(buf), error);
            if (error == asio::error::eof) {
                Log::Info("closed normally");
                break; // Connection closed cleanly by peer.
            } else if (error) {
                //throw std::system_error(error); // Some other error.
                Log::Error(std::format("closed w/ error: {}", error.message()));
                break;
            }

            Log::Info(std::format("Result: {}", std::string_view(buf.data(), len)));
        }
    // } catch(std::exception ex) {
    //     std::cerr << "EXCEPTION: " << ex.what();
    // }
    return 0;
}
