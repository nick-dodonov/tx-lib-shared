#include "Log/Log.h"
#include <asio.hpp>
#include <format>
#include <gtest/gtest.h>

static void print(const std::error_code& ec) {
    Log::Debug(std::format("Try: print: {}", ec.message()));
}

TEST(AsioTest, Try) {
    Log::Debug(std::format("Try: start: ASIO_HAS_PTHREADS={}", ASIO_HAS_PTHREADS));

    asio::io_context io;
    asio::steady_timer t(io, asio::chrono::milliseconds(200));
    t.async_wait(&print);
    io.run();

    Log::Debug("Try: end");
}
