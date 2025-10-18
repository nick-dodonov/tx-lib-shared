#include "Log/Log.h"
#include <gtest/gtest.h>
#include <coroutine>

struct CoroTest : testing::Test
{

};

TEST_F(CoroTest, Sub)
{
	Log::Debug("Sub1");

	//TODO: wrappers for co_await

	EXPECT_TRUE(true);
}
