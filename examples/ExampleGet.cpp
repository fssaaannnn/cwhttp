
#include <iostream>
#include "StringResponseBody.h"
#include "Request.h"
#include "Response.h"
#include "HttpClient.h"
#include "RequestBuilder.h"

int main(int argc, char** argv) {

    auto requestPtr = cwhttp::RequestBuilder()
            .setUrl("https://httpbin.org/get")
            .build();

    cwhttp::HttpClient hc;
    cwhttp::Response response;
    cwhttp::StringResponseBody responseBody;
    response.setBody(&responseBody);

    std::cout << "Succeeded: " << hc.execute(*requestPtr, response) << std::endl;
    std::cout << responseBody.getAsString() << std::endl;

    return 0;
}
