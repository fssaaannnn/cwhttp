
#include <gtest/gtest.h>
#include "UrlUtils.h"

TEST(UrlUtilsTest, TestEscapeUnescape) {
    const std::string ENCODED1 = "~%24test%20file.docx";
    const std::string DECODED1 = "~$test file.docx";

    ASSERT_EQ(ENCODED1, cwhttp::UrlUtils::escape(DECODED1));
    ASSERT_EQ(DECODED1, cwhttp::UrlUtils::unescape(ENCODED1));

    const std::string ENCODED2 = "%E3%83%86%E3%82%B9%E3%83%88%E7%94%A8%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB%20%28Example%29.txt";
    const std::string DECODED2 = "テスト用ファイル (Example).txt";

    ASSERT_EQ(ENCODED2, cwhttp::UrlUtils::escape(DECODED2));
    ASSERT_EQ(DECODED2, cwhttp::UrlUtils::unescape(ENCODED2));
}