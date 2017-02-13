
#include <gtest/gtest.h>
#include "StringUtils.h"

TEST(StringUtilsTest, TestTrim) {
    std::string empty;
    cwhttp::StringUtils::trim(empty);
    ASSERT_EQ(std::size_t(0), empty.length());

    std::string left("  test");
    cwhttp::StringUtils::trim(left);
    ASSERT_EQ(std::string("test"), left);

    std::string right("test ");
    cwhttp::StringUtils::trim(right);
    ASSERT_EQ(std::string("test"), right);

    std::string all(" test  ");
    cwhttp::StringUtils::trim(all);
    ASSERT_EQ(std::string("test"), all);

    ASSERT_EQ("test", cwhttp::StringUtils::trimConst("  'test'  ",
            [](int c) { return cwhttp::StringUtils::isSpaceOrCtl(c) || c == '\''; }));
}

TEST(StringUtilsTest, TestHasPrefix) {
    ASSERT_TRUE(cwhttp::StringUtils::hasPrefix("test.txt", "test."));
    ASSERT_FALSE(cwhttp::StringUtils::hasPrefix("test1.doc", "test."));
}

TEST(StringUtilsTest, TestHasSuffix) {
    ASSERT_TRUE(cwhttp::StringUtils::hasSuffix("test.txt", ".txt"));
    ASSERT_FALSE(cwhttp::StringUtils::hasSuffix("test.doc", ".txt"));
    ASSERT_TRUE(cwhttp::StringUtils::hasSuffix("Accept-Language  : en\x0D\x0A", "\x0D\x0A"));
    ASSERT_FALSE(cwhttp::StringUtils::hasSuffix("Accept-Language  : en\x0D\x0A", "\x0D\x0A\x0D\x0A"));
}

TEST(StringUtilsTest, TestEqualIgnoreCase) {
    ASSERT_TRUE(cwhttp::StringUtils::equalIgnoreCase("", ""));
    ASSERT_TRUE(cwhttp::StringUtils::equalIgnoreCase("C", "c"));
    ASSERT_TRUE(cwhttp::StringUtils::equalIgnoreCase("c", "C"));
    ASSERT_TRUE(cwhttp::StringUtils::equalIgnoreCase("C", "C"));
    ASSERT_TRUE(cwhttp::StringUtils::equalIgnoreCase("c", "c"));
    ASSERT_TRUE(cwhttp::StringUtils::equalIgnoreCase("AbC", "ABC"));
    ASSERT_TRUE(cwhttp::StringUtils::equalIgnoreCase("abc", "aBc"));
    ASSERT_FALSE(cwhttp::StringUtils::equalIgnoreCase("ccc", "ccb"));
    ASSERT_FALSE(cwhttp::StringUtils::equalIgnoreCase("acc", "ccc"));
    ASSERT_FALSE(cwhttp::StringUtils::equalIgnoreCase("cc", "ccc"));
    ASSERT_FALSE(cwhttp::StringUtils::equalIgnoreCase("ccc", "cc"));
}

