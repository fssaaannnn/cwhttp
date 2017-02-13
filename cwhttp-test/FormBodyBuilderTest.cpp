
#include <gtest/gtest.h>
#include "FormBodyBuilder.h"
#include "StringRequestBody.h"

TEST(FormBodyBuilderTest, TestBuild) {
    const std::string EXPECTED = "t1=v1&t2=v2&t3=~%24test%20file.docx";
    cwhttp::FormBodyBuilder builder;
    auto body = builder.addEncoded("t1", "v1")
            .add("t2", "v2")
            .add("t3", "~$test file.docx").build();

    std::vector<char> buffer(body->getContentLength());
    body->onRead(buffer.data(), EXPECTED.length());

    std::string bodyString(buffer.data(), buffer.size());

    ASSERT_EQ(EXPECTED.size(), body->getContentLength());
    ASSERT_EQ(EXPECTED.size(), bodyString.size());
    ASSERT_EQ(EXPECTED, bodyString);
}

TEST(FormBodyBuilderTest, TestBuildString) {
    const std::string EXPECTED = "t1=v1&t2=v2&t3=~%24test%20file.docx";
    cwhttp::FormBodyBuilder builder;
    std::string bodyString = builder.addEncoded("t1", "v1")
            .add("t2", "v2")
            .add("t3", "~$test file.docx").buildString();

    ASSERT_EQ(EXPECTED, bodyString);
}

TEST(FormBodyBuilderTest, TestReset) {
    cwhttp::FormBodyBuilder builder;
    builder.addEncoded("t1", "v1")
            .add("t2", "v2")
            .add("t3", "v3");
    builder.reset();
    auto body = builder.build();

    std::vector<char> buffer(body->getContentLength());
    body->onRead(buffer.data(), 777);

    std::string bodyString(buffer.data(), buffer.size());

    ASSERT_EQ(0, body->getContentLength());
    ASSERT_EQ(0, bodyString.size());
    ASSERT_EQ("", bodyString);
}