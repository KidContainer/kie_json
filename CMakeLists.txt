cmake_minimum_required(VERSION 3.15)
project(kie_json CXX)

set(CMAKE_CXX_STANDARD 20)

include(CTest)

find_package(Boost REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(GTest REQUIRED)

option(ENABLE_TEST "If enable test to compile and run test, the coverage will also be generated" OFF)

if(ENABLE_TEST)
    add_compile_options(--coverage -g -Wall -Werror)
    add_link_options(--coverage)
    add_subdirectory(test)
endif()

add_library(kie_json INTERFACE)
target_include_directories(kie_json INTERFACE include)
target_link_libraries(kie_json INTERFACE boost::boost nlohmann_json::nlohmann_json gtest::gtest)