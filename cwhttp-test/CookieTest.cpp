
#include <gtest/gtest.h>
#include "Cookie.h"

TEST(CookieTest, TestEmpty) {
    cwhttp::Cookie c("");

    ASSERT_EQ("", c.getName());
    ASSERT_EQ("", c.getValue());
}

TEST(CookieTest, TestInvalidEmpty) {
    cwhttp::Cookie c(";");

    ASSERT_EQ("", c.getName());
    ASSERT_EQ("", c.getValue());
}

TEST(CookieTest, TestSimple) {
    cwhttp::Cookie c("test=123");

    ASSERT_EQ("test", c.getName());
    ASSERT_EQ("123", c.getValue());

    cwhttp::Cookie cc("test=123;");

    ASSERT_EQ("test", cc.getName());
    ASSERT_EQ("123", cc.getValue());
}

TEST(CookieTest, TestSimpleInvalid) {
    cwhttp::Cookie c("test=123;;");

    ASSERT_EQ("test", c.getName());
    ASSERT_EQ("123", c.getValue());

    cwhttp::Cookie cc("  ; ;; test=123;  ; ;");

    ASSERT_EQ("test", cc.getName());
    ASSERT_EQ("123", cc.getValue());
}

TEST(CookieTest, TestQuote) {
    cwhttp::Cookie c(R"(test="123")");

    ASSERT_EQ("test", c.getName());
    ASSERT_EQ("123", c.getValue());
}

TEST(CookieTest, TestJSessionId) {
    cwhttp::Cookie c(R"(JSESSIONID=AJGWL98N622NMOA2WZNJAKKGQY5RSOXN; Path=/)");

    ASSERT_EQ("JSESSIONID", c.getName());
    ASSERT_EQ("AJGWL98N622NMOA2WZNJAKKGQY5RSOXN", c.getValue());
    ASSERT_EQ("/", c.getPath());
    ASSERT_FALSE(c.isHttpOnly());
    ASSERT_FALSE(c.isSecure());
}

TEST(CookieTest, TestGoogleAppsCookie) {
    cwhttp::Cookie c(
            R"(SSID=xxxxxxxxxxxxxxx-1; Domain=.example.com; Path=/; Expires=Fri, 15-Mar-2019 11:42:36 GMT; Secure; HttpOnly; Priority=HIGH)");

    ASSERT_EQ("SSID", c.getName());
    ASSERT_EQ("xxxxxxxxxxxxxxx-1", c.getValue());
    ASSERT_EQ("Fri, 15-Mar-2019 11:42:36 GMT", c.getExpires());
    ASSERT_EQ(".example.com", c.getDomain());
    ASSERT_EQ("/", c.getPath());
    ASSERT_TRUE(c.isHttpOnly());
    ASSERT_TRUE(c.isSecure());
}