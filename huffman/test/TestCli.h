//
// Created by mark on 4/30/20.
//

#ifndef HW_03_TESTCLI_H
#define HW_03_TESTCLI_H

#include <iostream>
#include <doctest/doctest.h>
#include <sstream>
#include "CLI.h"

TEST_CASE ("assert right arguments with no exceptions") {
    bool zip = false;
    std::string fin, fout;

    try {
        int argc = 6;
        const char* argv[6] = {"./program", "-c", "-f", "fin", "-o", "fout"};

        CLI::set_flags(zip, fin, fout, argc, const_cast<char**>(argv));
                CHECK(zip);
                CHECK(fin == "fin");
                CHECK(fout == "fout");
    } catch (...) {
        // it should not get here
                CHECK(false);
    }
}

TEST_CASE ("assert wrong argument exception") {
    bool zip = false;
    std::string fin, fout;

    try {
        int argc = 6;
        const char* argv[6] = {"./program", "-k", "-f", "fin", "-o", "fout"};

        CLI::set_flags(zip, fin, fout, argc, const_cast<char**>(argv));
        // it should not get here
                CHECK(false);
    } catch (std::exception& e) {
        std::string warning = e.what();
        CHECK(warning == "bad argument.\nTry " + CliException::perfect_input);
    }
}

#endif //HW_03_TESTCLI_H
