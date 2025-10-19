#pragma once
#include <chrono>
#include <coroutine>
#include <queue>
#include <thread>

struct QueueSynCtx {
    using Task = std::coroutine_handle<>;

    void post(Task handle) { tasks.push(handle); }

    bool run_once() {
        if (tasks.empty()) {
            return false;
        }
        auto handle = tasks.front();
        tasks.pop();
        handle.resume();
        return true;
    }

    void run() {
        while (run_once()) {
            ; // continue running tasks
        }
    }

    [[nodiscard]] bool empty() const noexcept { return tasks.empty(); }

private:
    std::queue<Task> tasks;
};

struct DelayAwaiter {
    std::chrono::milliseconds delay;
    QueueSynCtx* synCtx;

    [[nodiscard]] bool await_ready() const noexcept { return false; }
    void await_suspend(std::coroutine_handle<> handle) const {
        // Emulate async — "sleep" in a separate thread, then schedule resumption in synCtx.
        std::thread([handle, duration = delay, synCtx = this->synCtx] {
            std::this_thread::sleep_for(duration);
            synCtx->post(handle);
        }).detach();
    }
    void await_resume() const noexcept {}
};

inline DelayAwaiter delay_for(std::chrono::milliseconds duration, QueueSynCtx& synCtx) {
    return {duration, &synCtx};
}
