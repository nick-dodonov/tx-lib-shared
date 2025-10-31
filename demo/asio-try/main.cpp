#include "Boot/Boot.h"
#include "Log/Log.h"
#include <asio.hpp>

using namespace std::chrono_literals;

void print(const std::error_code& ec)
{
    Log::Info(std::format("Asio callback: {}.{}", ec.category().name(), ec.message()));
}

int main(int argc, char** argv)
{
    Boot::LogInfo(argc, argv);
    asio::io_context io;
    asio::steady_timer t(io, 5s);

    //t.wait();
    t.async_wait(&print);
    io.run();

    Log::Info("waited");
    return 0;
}
