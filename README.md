# cwhttp
[libcurl][] Wrapper Http client 

## About
This is a C++ HTTP client wrapping [libcurl][]

## Usage

```cpp

auto requestPtr = cwhttp::RequestBuilder()
        .setUrl("https://httpbin.org/get")
        .build();

cwhttp::HttpClient hc;
cwhttp::Response response;
cwhttp::StringResponseBody responseBody;
response.setBody(&responseBody);

hc.execute(*requestPtr, response);
std::cout << responseBody.getAsString() << std::endl;

```

## Dependencies
- [libcurl][]
- [googletest][]


[libcurl]: http://curl.haxx.se/libcurl/
[googletest]: https://github.com/google/googletest
