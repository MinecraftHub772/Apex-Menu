cmake_minimum_required(VERSION 3.25)

project(ApexMenu VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

setup_geode_mod(ApexMenu)

target_sources(ApexMenu PRIVATE 
    src/main.cpp
)

target_include_directories(ApexMenu PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(ApexMenu PUBLIC geode-sdk)
