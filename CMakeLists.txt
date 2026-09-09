cmake_minimum_required(VERSION 3.25)

project(ApexMenu VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# Android-specific settings
if(ANDROID)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    
    # Support both 32-bit and 64-bit
    if(ANDROID_ABI STREQUAL "armeabi-v7a")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -marm")
    elseif(ANDROID_ABI STREQUAL "arm64-v8a")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8-a")
    endif()
endif()

setup_geode_mod(ApexMenu)

target_sources(ApexMenu PRIVATE 
    src/main.cpp
)

target_include_directories(ApexMenu PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(ApexMenu PUBLIC geode-sdk)

# Set optimization flags
if(MSVC)
    target_compile_options(ApexMenu PRIVATE /O2)
else()
    target_compile_options(ApexMenu PRIVATE -O2)
endif()
