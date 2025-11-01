#include "Boot/Boot.h"
#include "Log/Log.h"
#include <asio.hpp>
#include <functional>

using namespace std::chrono_literals;

class printer
{
public:
    printer(asio::io_context& io)
        : strand_(asio::make_strand(io))
        , timer1_(io, asio::chrono::seconds(1))
        , timer2_(io, asio::chrono::seconds(1))
        , count_(0)
    {
        timer1_.async_wait(asio::bind_executor(strand_,
            std::bind(&printer::print, this, asio::placeholders::error, &timer1_)));
        timer2_.async_wait(asio::bind_executor(strand_,
            std::bind(&printer::print, this, asio::placeholders::error, &timer2_)));
    }
    ~printer() {
        Log::Info(std::format("Final count is {}", count_));
    }

    void print(const std::error_code& ec, asio::steady_timer* timer)
    {
        if (count_ < 5) {
            Log::Info(std::format("print: [{}] {}", count_, ec.message()));
            ++count_;

            timer->expires_at(timer->expiry() + asio::chrono::seconds(1));
            timer->async_wait(asio::bind_executor(strand_,
                std::bind(&printer::print, this, asio::placeholders::error, timer)));
        }
    }

private:
  asio::strand<asio::io_context::executor_type> strand_;
  asio::steady_timer timer1_;
  asio::steady_timer timer2_;
  int count_;
};


int main(int argc, char** argv)
{
    Boot::LogInfo(argc, argv);

    asio::io_context io;
    printer p(io);
    std::thread t([&]{ io.run(); });
    io.run();
    t.join();

    return 0;
}
