
#include <gtest/gtest.h>
#include "HeaderContainer.h"

class HeaderContainerTest : public testing::Test {
protected:
    cwhttp::HeaderContainer m_Hc;

    virtual void SetUp() override {
    }

    virtual void TearDown() override {
    }

};

TEST_F(HeaderContainerTest, TestParseLine) {
    std::string name;
    std::string value;

    ASSERT_FALSE(cwhttp::HeaderContainer::parseLine(" Accept-Language  :", name, value));
    ASSERT_EQ("", name);
    ASSERT_EQ("", value);

    ASSERT_FALSE(cwhttp::HeaderContainer::parseLine(":   en\x0D\x0A", name, value));
    ASSERT_EQ("", name);
    ASSERT_EQ("", value);

    ASSERT_TRUE(cwhttp::HeaderContainer::parseLine(" Accept-Language  :   en\x0D\x0A", name, value));
    ASSERT_EQ("Accept-Language", name);
    ASSERT_EQ("en", value);

    ASSERT_TRUE(cwhttp::HeaderContainer::parseLine("Set-Cookie: ABC=123; Path=/", name, value));
    ASSERT_EQ("Set-Cookie", name);
    ASSERT_EQ("ABC=123; Path=/", value);
}

TEST_F(HeaderContainerTest, TestAddLine) {
    ASSERT_FALSE(m_Hc.has("Accept-Language"));
    m_Hc.addLine("Accept-Language: en");
    ASSERT_TRUE(m_Hc.has("Accept-Language"));
    ASSERT_EQ("en", std::string(m_Hc.get("Accept-Language")));
    ASSERT_EQ(std::size_t(1), m_Hc.getSize());
}

TEST_F(HeaderContainerTest, TestAdd) {
    m_Hc.addLine("Set-Cookie: ABC=123; Path=/");
    m_Hc.addRaw("Set-Cookie", " DEF=456; Path=/ ");
    ASSERT_EQ(std::size_t(2), m_Hc.getSize());
}

TEST_F(HeaderContainerTest, TestAddAndTrim) {
    ASSERT_FALSE(m_Hc.has("Accept-Language"));
    m_Hc.addLine(" Accept-Language  :   en\x0D\x0A");
    ASSERT_TRUE(m_Hc.has("Accept-Language"));
    ASSERT_EQ("en", std::string(m_Hc.get("Accept-Language")));
    ASSERT_EQ(std::size_t(1), m_Hc.getSize());
}

TEST_F(HeaderContainerTest, TestIterateAll) {
    const char* HEADER_LINES[] = {
            "Set-Cookie:   ABC=123  ;  Path=/   ",
            " Accept-Language  :   en\x0D\x0A",
            "User-Agent: test ua   \x0D\x0A"
    };
    const char* EXPECTED_LINES[] = {
            "Accept-Language: en",
            "Set-Cookie: ABC=123  ;  Path=/",
            "User-Agent: test ua"
    };

    for (auto header:HEADER_LINES) {
        m_Hc.addLine(header);
    }
    int i = 0;
    m_Hc.iterateAllLines([&](const std::string& headerLine) {
        std::string expected(EXPECTED_LINES[i]);
        ASSERT_EQ(expected, headerLine);
        ++i;
    });
}