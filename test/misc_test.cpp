#include "Log/Log.h"
#include <gtest/gtest.h>

TEST(MiscTest, Foo) {
    Log::Debug("Test message");
    EXPECT_TRUE(true);
}
