//
// Created by mark on 4/20/20.
//

#ifndef HW_03_TESTCASES_H
#define HW_03_TESTCASES_H

#include <iostream>
#include <doctest/doctest.h>
#include <sstream>
#include "HuffmanArchiver.h"

TEST_CASE ("compression of thousand same symbols") {
    std::stringstream in;
    std::stringstream out;

    for (int i = 0; i < 1000; i++) {
        in << "a";
    }
    auto n = HuffmanArchiver::compress(in, out);
    out.seekg(0, std::ios::end);
    int size = out.tellg();
            CHECK(size - n == 1000 / 8);
}

TEST_CASE ("compression of empty file") {
    std::stringstream in;
    std::stringstream out;

    auto n = HuffmanArchiver::compress(in, out);
    out.seekg(0, std::ios::end);
    int size = out.tellg();

            CHECK(size - n == 0);
            CHECK(size == 0);
            CHECK(n == 0);
}

TEST_CASE ("compress + extract thousand same symbols") {
    std::stringstream in;
    std::stringstream out;

    for (int i = 0; i < 1; i++) {
        in << "a";
    }
    HuffmanArchiver::compress(in, out);

    std::stringstream new_out;

    HuffmanArchiver::extract(out, new_out);
    for (int i = 0; i < 1; i++) {
        char c;
        new_out >> c;
                CHECK(c == 'a');
    }
}


TEST_CASE ("compress + extract different symbols") {
    std::stringstream in;
    std::stringstream out;

    for (int i = 0; i < 1000; i++) {
        in << char(i);
    }
    HuffmanArchiver::compress(in, out);

    std::stringstream new_out;

    HuffmanArchiver::extract(out, new_out);
    for (int i = 0; i < 1000; i++) {
        char c;
        new_out.get(c);
                CHECK(c == char(i));
    }
}


TEST_CASE ("compression of empty file") {
    std::stringstream in;
    std::stringstream out;

    auto n = HuffmanArchiver::compress(in, out);
    out.seekg(0, std::ios::end);
    int size = out.tellg();

            CHECK(size - n == 0);
            CHECK(size == 0);
            CHECK(n == 0);
}

#endif //HW_03_TESTCASES_H
