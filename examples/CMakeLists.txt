cmake_minimum_required(VERSION 3.4)
project(examples)
set(CMAKE_VERBOSE_MAKEFILE on)

add_definitions(-std=c++11)

include_directories(../cwhttp)

add_executable(ExampleGet ExampleGet.cpp)
target_link_libraries(ExampleGet cwhttp)

add_executable(ExamplePostForm ExamplePostForm.cpp)
target_link_libraries(ExamplePostForm cwhttp)

if (MSVC)
    # Force to always compile with W4
    if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif ()
elseif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    # Update if necessary
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif ()
