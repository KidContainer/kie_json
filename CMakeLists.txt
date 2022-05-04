cmake_minimum_required(VERSION 3.15)
project(kie_json CXX)

set(CMAKE_CXX_STANDARD 20)

include(CTest)

find_package(Boost REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(GTest REQUIRED)

add_subdirectory(test)

add_library(kie_json INTERFACE)
target_include_directories(kie_json INTERFACE include)
target_link_libraries(kie_json INTERFACE boost::boost nlohmann_json::nlohmann_json gtest::gtest)