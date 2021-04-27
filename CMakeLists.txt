cmake_minimum_required(VERSION 3.5.1)
project(psf)

option(BUILD_TESTING "" OFF)
option(RUN_CLANG_TIDY "" OFF)

# WARN: This project relies on atomic_flag wait available only in C++20 and libc++.
# One must have clang 11+ to compile.
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -Wconversion -Wall -Werror -Wextra -Wpedantic")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(CTest)

if (RUN_CLANG_TIDY)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;--warnings-as-errors=*")
# Disable clang-tidy checks for auto-generated files.
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/.clang-tidy" "---\nChecks: '-*,llvm-twine-local'\n...\n")
endif()

add_subdirectory(psf_core)
add_subdirectory(psf_perf_tests)
add_subdirectory(psf_examples)
