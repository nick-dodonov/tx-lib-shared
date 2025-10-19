#include "CoroTest.h"

TEST_F(CoroTest, Sub) {
    Log::Debug("Sub: start");
    co_await delay_for(std::chrono::milliseconds(500), synCtx);
    Log::Debug("Sub: after delay");
    co_await delay_for(std::chrono::milliseconds(500), synCtx);
    Log::Debug("Sub: end");
}
