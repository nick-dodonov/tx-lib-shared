#include <gtest/gtest.h>
#include "Shared/Version.h"

class VersionTest : public ::testing::Test {};

TEST_F(VersionTest, GetVersionDetailed_ReturnsNonEmptyString) {
    std::string version = GetVersionDetailed();
    EXPECT_FALSE(version.empty());
}

TEST_F(VersionTest, GetVersionDetailed_ContainsVersionKeyword) {
    std::string version = GetVersionDetailed();
    EXPECT_NE(version.find("Version"), std::string::npos);
}

TEST_F(VersionTest, GetVersionDetailed_IsConsistent) {
    std::string version1 = GetVersionDetailed();
    std::string version2 = GetVersionDetailed();
    EXPECT_EQ(version1, version2);
}

TEST_F(VersionTest, GetVersionDetailed_ContainsExpectedFormat) {
    std::string version = GetVersionDetailed();
    // Check that it contains version number pattern (e.g., "0.1.0")
    EXPECT_TRUE(version.find(".") != std::string::npos);
}
