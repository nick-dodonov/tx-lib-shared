#include "Log/Log.h"
#include <gtest/gtest.h>
#include <coroutine>
#include <queue>
#include <iostream>
#include <chrono>
#include <thread>

struct SimpleLoop {
    using Task = std::coroutine_handle<>;
    void post(Task h) { tasks.push(h); }
    void run() {
        while (!tasks.empty()) {
            auto h = tasks.front();
            tasks.pop();
            h.resume();
        }
    }
private:
    std::queue<Task> tasks;
};

inline SimpleLoop g_loop; // глобальный планировщик

struct DelayAwaiter {
    std::chrono::milliseconds delay;
    bool await_ready() const noexcept { return false; }
    void await_suspend(std::coroutine_handle<> h) const {
        // Имитируем асинхронность — "засыпаем" в отдельном потоке,
        // потом планируем возобновление в loop.
        std::thread([h, d=delay]{
            std::this_thread::sleep_for(d);
            g_loop.post(h);
        }).detach();
    }
    void await_resume() const noexcept {}
};

inline DelayAwaiter delay_for(std::chrono::milliseconds d) { return {d}; }

struct GTestPromise {
    std::coroutine_handle<> continuation = nullptr;
    std::suspend_never initial_suspend() noexcept { return {}; }
    // Финальная приостановка сообщает loop'у: “всё, тест закончен”.
    struct FinalAwaiter {
        bool await_ready() noexcept { return false; }
        void await_suspend(std::coroutine_handle<GTestPromise> h) noexcept {
            if (h.promise().continuation)
                g_loop.post(h.promise().continuation);
        }
        void await_resume() noexcept {}
    };
    FinalAwaiter final_suspend() noexcept { return {}; }
    void get_return_object() noexcept {}
    void return_void() noexcept {}
    void unhandled_exception() noexcept { std::terminate(); }
};

class FooFixture : public ::testing::Test {
protected:
    void RunCoroutine(std::coroutine_handle<GTestPromise> h) {
        // запоминаем continuation = handle main'а (или RunLoop)
        h.promise().continuation = std::noop_coroutine();
        g_loop.post(h);
        g_loop.run(); // блокируемся пока тест не завершится
    }
};

// --- Правильная шаблонная специализация std::coroutine_traits с концептом ---
namespace std {
	template<typename T>
	requires std::is_base_of_v<FooFixture, T>
	struct coroutine_traits<void, T&> {
		using promise_type = GTestPromise;
	};
} // namespace std

#define TEST_F_CORO(fixture, name)                                  \
    class fixture##_##name##_Test : public fixture {                \
    public:                                                         \
        void TestBody() override { run_coro(); }                    \
        void run_coro();                                            \
    };                                                              \
    void fixture##_##name##_Test::run_coro()

TEST_F_CORO(FooFixture, Bar)
{
    std::cout << "Before await\n";
    co_await delay_for(std::chrono::milliseconds(200));
    std::cout << "After 200ms\n";
}
