cmake_minimum_required(VERSION 3.4)
project(cwhttp)
set(CMAKE_VERBOSE_MAKEFILE on)

add_definitions(-std=c++11)

find_package(CURL)
include_directories(${CURL_INCLUDE_DIRS})
include_directories(../cwlog)

set(SOURCE_FILES
        Method.h
        Header.h
        HttpClient.cpp HttpClient.h
        ScopedCurl.cpp ScopedCurl.h
        Request.cpp Request.h
        RequestBody.cpp RequestBody.h
        Response.cpp Response.h
        ResponseBody.cpp ResponseBody.h
        StringResponseBody.cpp StringResponseBody.h
        HeaderContainer.cpp HeaderContainer.h
        StringUtils.cpp StringUtils.h
        ScopedSList.cpp ScopedSList.h
        FormBodyBuilder.cpp FormBodyBuilder.h
        StringRequestBody.cpp StringRequestBody.h
        RequestBuilder.cpp RequestBuilder.h
        Cookie.cpp Cookie.h
        CookieJar.cpp CookieJar.h
        UrlUtils.cpp UrlUtils.h)

add_library(cwhttp STATIC ${SOURCE_FILES})
target_link_libraries(cwhttp cwlog ${CURL_LIBRARIES} ssl crypto z)

if (MSVC)
    # Force to always compile with W4
    if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif ()
elseif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter")
endif ()
