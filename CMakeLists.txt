cmake_minimum_required(VERSION 3.25)
project(Password_Manager)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(Password_Manager
        main.cpp)
include(FetchContent)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 10.1.0
)
FetchContent_MakeAvailable(fmt)
add_executable(1 main.cpp)
target_link_libraries(1 fmt)
