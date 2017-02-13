
#include <iostream>
#include "FormBodyBuilder.h"
#include "StringResponseBody.h"
#include "Request.h"
#include "RequestBody.h"
#include "Response.h"
#include "HttpClient.h"
#include "Method.h"
#include "RequestBuilder.h"

int main(int argc, char** argv) {

    auto requestBodyPtr = cwhttp::FormBodyBuilder()
            .addEncoded("fieldName1", "fieldValue1")
            .add("fieldName2", "テストフィールド2")
            .build();

    auto requestPtr = cwhttp::RequestBuilder()
            .setMethod(cwhttp::Method::POST)
            .setBody(requestBodyPtr.get())
            .setUrl("https://httpbin.org/post")
            .build();

    cwhttp::HttpClient hc;
    cwhttp::Response response;
    cwhttp::StringResponseBody responseBody;
    response.setBody(&responseBody);

    std::cout << "Succeeded: " << hc.execute(*requestPtr, response) << std::endl;
    std::cout << responseBody.getAsString() << std::endl;

    return 0;
}
