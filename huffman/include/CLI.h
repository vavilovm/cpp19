//
// Created by mark on 4/18/20.
//

#ifndef HW_03_CLI_H
#define HW_03_CLI_H

#include <string>
#include <exception>
#include <utility>

class CliException : public std::exception {
public:
    explicit CliException(std::string msg) : msg_(std::move(msg)) {}

    [[nodiscard]] const char* what() const noexcept override;

    inline static const std::string perfect_input = "'-c' (to zip) or '-u' (to unzip) '-f' <filename> '-o' <filename>";;

private:
    std::string msg_;
};

class CLI {
public:
    static void set_flags(bool& zip, std::string& fin, std::string& fout, int argc, char* argv[]);
};


#endif //HW_03_CLI_H
