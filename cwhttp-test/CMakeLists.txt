cmake_minimum_required(VERSION 3.4)
project(cwhttp-test)
set(CMAKE_VERBOSE_MAKEFILE on)

add_definitions(-std=c++11)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(../cwhttp)

set(SOURCE_FILES
        TestEntry.cpp
        HttpClientTest.cpp
        StringUtilsTest.cpp
        HeaderContainerTest.cpp
        FormBodyBuilderTest.cpp
        CookieTest.cpp
        UrlUtilsTest.cpp)
add_executable(cwhttp-test ${SOURCE_FILES})

target_link_libraries(cwhttp-test ${GTEST_BOTH_LIBRARIES})
target_link_libraries(cwhttp-test cwhttp)


