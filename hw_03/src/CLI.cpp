//
// Created by mark on 4/18/20.
//

#include "CLI.h"

void CLI::set_flags(bool& zip, std::string& fin, std::string& fout, int argc, char* argv[]) {
    bool was_cmd = false;
    bool was_fin = false;
    bool was_fout = false;

    for (int i = 1; i < argc; i++) {
        std::string_view cmd = argv[i];

        if (cmd == "-c") {
            zip = true;
            was_cmd = true;
        } else if (cmd == "-u") {
            zip = false;
            was_cmd = true;
        } else if (cmd == "-f" || cmd == "-file") {
            if (++i >= argc) {
                throw CliException("no filename after \"-file\" command");
            }
            fin = argv[i];

            was_fin = true;
        } else if (cmd == "-o" || cmd == "--output") {

            if (++i < argc) {
                fout = argv[i];
            } else {
                throw CliException("no filename after \"-output\" command");
            }

            was_fout = true;
        } else {
            throw CliException("bad argument.\nTry " + CliException::perfect_input);
        }
    }
    if (!was_fout)
        throw CliException("no input file. Try to add '-f' <filename>. \nCorrect input: " +
                           CliException::perfect_input);
    if (!was_fin)
        throw CliException("no output file. Try to add '-o' <filename>. \nCorrect input: " +
                           CliException::perfect_input);
    if (!was_cmd)
        throw CliException("no command. Try to add '-c' (to zip) or '-u' (to unzip). \nCorrect input: " +
                           CliException::perfect_input);
}

const char* CliException::what() const noexcept {
    return msg_.c_str();
}
