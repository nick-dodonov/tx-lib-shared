#include "Boot/Boot.h"
#include "Log/Log.h"
#include <asio.hpp>
#include <functional>

using namespace std::chrono_literals;

class printer
{
public:
    printer(asio::io_context& io)
        : timer_(io, asio::chrono::seconds(1))
        , count_(0)
    {
        timer_.async_wait(std::bind(&printer::print, this, asio::placeholders::error));
    }
    ~printer() {
        Log::Info(std::format("Final count is {}", count_));
    }

    void print(const std::error_code& ec)
    {
        if (count_ < 5) {
            Log::Info(std::format("print: [{}] {}", count_, ec.message()));
            ++count_;

            timer_.expires_at(timer_.expiry() + asio::chrono::seconds(1));
            timer_.async_wait(std::bind(&printer::print, this, asio::placeholders::error));
        }
    }

private:
    asio::steady_timer timer_;
    int count_;    
};

void print(const std::error_code& ec, asio::steady_timer* t, int* count)
{
    Log::Info(std::format("Asio callback: [{}] {}.{}", *count, ec.category().name(), ec.message()));
    if (*count < 5) {
        ++(*count);
        t->expires_at(t->expiry() + asio::chrono::seconds(1));
        t->async_wait(std::bind(print, asio::placeholders::error, t, count));
    }
}

int main(int argc, char** argv)
{
    Boot::LogInfo(argc, argv);
    asio::io_context io;
    printer p(io);
    io.run();
    return 0;
}
