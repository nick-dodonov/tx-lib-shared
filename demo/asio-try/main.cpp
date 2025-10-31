#include "Boot/Boot.h"
#include "Log/Log.h"
#include <asio.hpp>

int main(int argc, char** argv)
{
    Boot::LogInfo(argc, argv);
    asio::io_context io;
    asio::steady_timer t(io, asio::chrono::seconds(5));
    t.wait();
    Log::Info("waited");
    return 0;
}
