cmake_minimum_required(VERSION 3.10.2)
project(hw_03)

set(CMAKE_CXX_STANDARD 17)
add_definitions("-Wall -Wextra")
include_directories(include)

add_executable(hw_03 src/main.cpp src/HuffmanArchiver.cpp
        include/HuffmanArchiver.h src/CLI.cpp include/CLI.h src/Huffman.cpp include/Huffman.h)

find_package(doctest CONFIG REQUIRED)

add_executable(test_hw_03 test/TestMain.cpp src/HuffmanArchiver.cpp
        include/HuffmanArchiver.h src/CLI.cpp include/CLI.h src/Huffman.cpp include/Huffman.h test/TestCases.h test/UnitTests.h test/TestCli.h)

target_link_libraries(test_hw_03 doctest::doctest)