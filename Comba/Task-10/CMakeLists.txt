cmake_minimum_required(VERSION 3.20)
project(SFML-Collection LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.5.1)
FetchContent_MakeAvailable(SFML)

if (WIN32)
    option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
    set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
endif ()

add_executable(${PROJECT_NAME} main.cpp
        Geometry.h)

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-audio)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
