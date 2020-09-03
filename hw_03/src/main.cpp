#include <iostream>
#include "CLI.h"
#include "HuffmanArchiver.h"

/*
 *
 * ./hw_03 -u -f result.bin -o new_text.txt
 * ./hw_03 -c -f myfile.txt -o result.bin
 *
 */


int main(int argc, char* argv[]) {
    try {
        bool zip = false;
        std::string fin_name, fout_name;

        CLI::set_flags(zip, fin_name, fout_name, argc, argv);

        std::ifstream fin(fin_name);
        std::ofstream fout(fout_name);

        size_t extra_info;


        if (zip) {
            extra_info = HuffmanArchiver::compress(fin, fout);
            fout.flush();
            fin.clear();
            fin.seekg(0, std::ios::end);
            long input_size = fin.tellg();
            if (input_size == -1) input_size = 0;
            std::cout << input_size << "\n" << ((size_t) fout.tellp() - extra_info) << "\n" << extra_info;
        } else {
            extra_info = HuffmanArchiver::extract(fin, fout);
            fout.flush();
            fin.clear();
            fin.seekg(0, std::ios::end);
            long input_size = fin.tellg();
            if (input_size == -1) input_size = 0;
            std::cout << input_size - extra_info << "\n" << fout.tellp() << "\n" << extra_info << "\n";
        }
    } catch (std::exception& e) {
        std::cout << e.what() << "\n";
    }

    return 0;
}
