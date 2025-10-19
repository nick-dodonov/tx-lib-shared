#pragma once
#include <chrono>
#include <coroutine>
#include <queue>
#include <thread>

template<typename T>
concept SynCtx = requires(T ctx, std::coroutine_handle<> handle) {
    ctx.post(handle);
};

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

    void run_all() {
        while (run_once()) {
            ; // continue running tasks
        }
    }

    [[nodiscard]] bool empty() const noexcept { return tasks.empty(); }

private:
    std::queue<Task> tasks;
};

static_assert(SynCtx<QueueSynCtx>, "QueueSynCtx must satisfy SynCtx concept");

template<SynCtx SynCtx>
struct DelayAwaiter {
    std::chrono::milliseconds delay;
    SynCtx* synCtx;

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

template<SynCtx SynCtx>
inline DelayAwaiter<SynCtx> delay_for(std::chrono::milliseconds duration, SynCtx& synCtx) {
    return {duration, &synCtx};
}
