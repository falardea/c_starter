cmake_minimum_required(VERSION 3.22)
project(c_starter VERSION 0.0.1)
set(PROJECT_TARGET c_starter)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_SOURCE_DIR ./src)
set(PROJECT_BINARY_DIR ./target)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

set(MY_SOURCES
    src/main.c
    src/common_defs.h
    src/version.h
    src/utils/parse_args.c
    src/utils/parse_args.h
    src/utils/logging.c
    src/utils/logging.h
    src/utils/sys_interface.c
    src/utils/sys_interface.h
    src/controllers/app_ctrl.c
    src/controllers/app_ctrl.h
    src/models/app_model.c
    src/models/app_model.h
    )

add_compile_options(-pedantic -Wall -Wextra)

include_directories(${PROJECT_SOURCE_DIR})

configure_file("${PROJECT_SOURCE_DIR}/version.h.in" "${PROJECT_SOURCE_DIR}/version.h")

add_executable(${PROJECT_TARGET} ${MY_SOURCES})

target_include_directories(${PROJECT_TARGET} PUBLIC "${PROJECT_BINARY_DIR}")

# Let's take a look at testing
# https://cmake.org/cmake/help/book/mastering-cmake/chapter/Testing%20With%20CMake%20and%20CTest.html
enable_testing()
