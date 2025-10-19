#include "Log/Log.h"
#include <gtest/gtest.h>

TEST(MiscTest, Foo) {
    Log::Debug("Test message");
    EXPECT_TRUE(true);
}

TEST(MiscTest, Foo2) {
    Log::Debug("Test message 2");
    EXPECT_TRUE(true);
}

TEST(MiscTest, Foo3) {
    Log::Debug("Test message 3");
    EXPECT_TRUE(true);
}
