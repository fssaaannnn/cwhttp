
#include <gtest/gtest.h>
#include <map>
#include "StringRequestBody.h"
#include "FormBodyBuilder.h"
#include "RequestBuilder.h"
#include "Request.h"
#include "RequestBody.h"
#include "Response.h"
#include "StringResponseBody.h"
#include "HttpClient.h"
#include "Method.h"
#include "Cookie.h"
#include "CookieJar.h"
#include "HeaderContainer.h"

class HttpClientTest : public testing::Test {
protected:
    cwhttp::HttpClient m_HttpClient;
    cwhttp::RequestBuilder m_Builder;
    cwhttp::Response m_Response;

    virtual void SetUp() override {
        m_HttpClient.setDebug(true);
    }

    virtual void TearDown() override {
    }

};

TEST_F(HttpClientTest, TestPostMethod) {
    cwhttp::FormBodyBuilder bodyBuilder;
    auto bodyPtr = bodyBuilder.addEncoded("testKey1", "testValue1")
            .addEncoded("testKey2", "testValue2")
            .build();

    auto requestPtr = m_Builder.setMethod(cwhttp::Method::POST)
            .setUrl("https://httpbin.org/post")
            .setBody(bodyPtr.get())
            .build();

    cwhttp::StringResponseBody body;
    m_Response.setBody(&body);

    ASSERT_TRUE(m_HttpClient.execute(*requestPtr, m_Response));
    ASSERT_EQ(200, m_Response.getCode());
    ASSERT_EQ(m_Response.getContentLength(), body.getAsString().length());
}

TEST_F(HttpClientTest, TestPutMethod) {
    cwhttp::StringRequestBody requestBody("text/plain", "test text");

    auto requestPtr = m_Builder.setMethod(cwhttp::Method::PUT)
            .setUrl("https://httpbin.org/put")
            .setBody(&requestBody)
            .build();

    cwhttp::StringResponseBody body;
    m_Response.setBody(&body);

    ASSERT_TRUE(m_HttpClient.execute(*requestPtr, m_Response));
    ASSERT_EQ(200, m_Response.getCode());
    ASSERT_EQ(m_Response.getContentLength(), body.getAsString().length());
}

TEST_F(HttpClientTest, TestPutEmptyBody) {
    cwhttp::StringRequestBody requestBody("text/plain", "");

    auto requestPtr = m_Builder.setMethod(cwhttp::Method::PUT)
            .setUrl("https://httpbin.org/put")
            .setBody(&requestBody)
            .build();

    cwhttp::StringResponseBody body;
    m_Response.setBody(&body);

    ASSERT_TRUE(m_HttpClient.execute(*requestPtr, m_Response));
    ASSERT_EQ(200, m_Response.getCode());
    ASSERT_EQ(m_Response.getContentLength(), body.getAsString().length());
}

TEST_F(HttpClientTest, TestPutChunked) {
    cwhttp::StringRequestBody requestBody("text/plain", "test text");

    auto requestPtr = m_Builder.setMethod(cwhttp::Method::PUT)
            .setUrl("https://httpbin.org/put")
            .setBody(&requestBody)
            .setChunked(true)
            .build();

    cwhttp::StringResponseBody body;
    m_Response.setBody(&body);

    ASSERT_TRUE(m_HttpClient.execute(*requestPtr, m_Response));
    ASSERT_EQ(200, m_Response.getCode());
    ASSERT_EQ(m_Response.getContentLength(), body.getAsString().length());
}

TEST_F(HttpClientTest, TestPatchMethod) {
    cwhttp::StringRequestBody requestBody("text/plain", "test text");

    auto requestPtr = m_Builder.setMethod(cwhttp::Method::PATCH)
            .setUrl("https://httpbin.org/patch")
            .setBody(&requestBody)
            .build();

    cwhttp::StringResponseBody body;
    m_Response.setBody(&body);

    ASSERT_TRUE(m_HttpClient.execute(*requestPtr, m_Response));
    ASSERT_EQ(200, m_Response.getCode());
    ASSERT_EQ(m_Response.getContentLength(), body.getAsString().length());
}

TEST_F(HttpClientTest, TestDeleteMethod) {
    auto requestPtr = m_Builder.setMethod(cwhttp::Method::DEL)
            .setUrl("https://httpbin.org/delete")
            .build();

    cwhttp::StringResponseBody body;
    m_Response.setBody(&body);

    ASSERT_TRUE(m_HttpClient.execute(*requestPtr, m_Response));
    ASSERT_EQ(200, m_Response.getCode());
    ASSERT_EQ(m_Response.getContentLength(), body.getAsString().length());
}

TEST_F(HttpClientTest, TestHeadMethod) {
    auto requestPtr = m_Builder.setMethod(cwhttp::Method::HEAD)
            .setUrl("https://httpbin.org/response-headers?Test-Header-Name=TestValue")
            .build();

    ASSERT_TRUE(m_HttpClient.execute(*requestPtr, m_Response));
    ASSERT_EQ(200, m_Response.getCode());
    ASSERT_EQ(m_Response.getContentLength(), 0);
    ASSERT_TRUE(m_Response.getHeaders()->has("Test-Header-Name"));
    ASSERT_STREQ("TestValue", m_Response.getHeaders()->get("Test-Header-Name"));
}

using CookieMap = std::map<std::string, cwhttp::Cookie>;

class AdaptedCookieJar : public cwhttp::CookieJar {
public:
    CookieMap* m_pCookies;

    AdaptedCookieJar(CookieMap* pCookies)
            : m_pCookies(pCookies) {

    }

    void saveFromResponse(const std::string& url, const cwhttp::CookieList& cookies) override {
        for (auto& c:cookies) {
            m_pCookies->emplace(c.getName(), c);
        }
    }

    void loadForRequest(const std::string& url, cwhttp::CookieList& cookies) const override {
        for (auto& pair:*m_pCookies) {
            cookies.emplace_back(pair.second);
        }
    }
};

TEST_F(HttpClientTest, TestSaveCookieJar) {
    auto requestPtr = m_Builder.setMethod(cwhttp::Method::GET)
            .setUrl("https://httpbin.org/cookies/set?testKey1=testValue1&testKey2=testValue2")
            .setFollowRedirect(true)
            .build();

    cwhttp::StringResponseBody body;
    m_Response.setBody(&body);

    CookieMap cookies;
    AdaptedCookieJar cookieJar(&cookies);
    m_HttpClient.setCookieJar(&cookieJar);
    ASSERT_TRUE(m_HttpClient.execute(*requestPtr, m_Response));
    ASSERT_EQ(200, m_Response.getCode());
    ASSERT_EQ(m_Response.getContentLength(), body.getAsString().length());
    ASSERT_EQ(2, cookies.size());
    ASSERT_EQ("testValue1", cookies.at("testKey1").getValue());
    ASSERT_EQ("testValue2", cookies.at("testKey2").getValue());

    requestPtr = m_Builder.setUrl("https://httpbin.org/cookies").build();
    m_Response.reset();
    ASSERT_TRUE(m_HttpClient.execute(*requestPtr, m_Response));
    ASSERT_EQ(200, m_Response.getCode());
    ASSERT_TRUE(body.getAsString().find("testValue1") != std::string::npos);
    ASSERT_TRUE(body.getAsString().find("testValue2") != std::string::npos);
}
