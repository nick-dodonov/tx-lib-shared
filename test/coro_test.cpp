#include "CoroTest.h"
#include <asio.hpp>
#include <format>

TEST_F(CoroTest, Sub) {
    Log::Debug("Sub: start");
    co_await delay_for(std::chrono::milliseconds(200), synCtx);
    Log::Debug("Sub: after delay");
    co_await delay_for(std::chrono::milliseconds(200), synCtx);
    Log::Debug("Sub: end");
}

static void print(const std::error_code& ec) {
    Log::Debug(std::format("Try: print: {}", ec.message()));
}

TEST(AsioTest, Try) {
    Log::Debug("Try: start");

    asio::io_context io;
    asio::steady_timer t(io, asio::chrono::milliseconds(200));
    t.async_wait(&print);
    io.run();

    Log::Debug("Try: end");
}
