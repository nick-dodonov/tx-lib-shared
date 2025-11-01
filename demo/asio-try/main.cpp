#include "Boot/Boot.h"
#include <array>
#include <iostream>
#include <asio.hpp>

using asio::ip::tcp;

int main(int argc, char** argv)
{
    Boot::LogInfo(argc, argv);

    // try {
        if (argc != 2) {
            std::cerr << "Usage: client <host>" << std::endl;
            std::cerr << "Service can be emulated: `while true; do echo -ne \"$(date -u)\\0\" | nc -l 13; echo accessed; done`" << std::endl;
            return 1;
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
                std::cout << "closed normally\n";
                break; // Connection closed cleanly by peer.
            } else if (error) {
                //throw std::system_error(error); // Some other error.
                std::cerr << "closed w/ error: " << error.message() << std::endl;
                break;
            }

            std::cout.write(buf.data(), len);
            std::cout << "\n";
        }
    // } catch(std::exception ex) {
    //     std::cerr << "EXCEPTION: " << ex.what();
    // }
    return 0;
}
