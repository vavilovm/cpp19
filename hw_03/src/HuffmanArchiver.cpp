//
// Created by mark on 4/18/20.
//

#include "../include/HuffmanArchiver.h"
#include "Huffman.h"
#include <unordered_map>
#include <vector>

#include <iostream>

size_t HuffmanArchiver::compress(std::istream& fin, std::ostream& fout) {
    // count number of each letter
    fin.exceptions(std::ifstream::badbit);
    std::unordered_map<char, int> letters;
    char c;
    while (fin.get(c))
        letters[c]++;

    int32_t n = letters.size();
    if (!n) return 0;

    // remember code of every symbol
    std::unordered_map<char, std::string> dict;

    int32_t freq_sum = HuffmanTree::zip(letters, dict);

    // go back
    fin.clear();
    fin.seekg(0, std::ios_base::beg);


    // remember stuff
    fout.write(reinterpret_cast<const char*>(&n), sizeof(n));
    fout.write(reinterpret_cast<const char*>(&freq_sum), sizeof(freq_sum));

    for (const auto&[ch, k] : dict)
        fout << ch << k << ' ';


    const size_t length_of_extra_information = fout.tellp();

    unsigned int cnt = 0;
    unsigned char byte = 0;
    while (fin.get(c)) {
        for (char ch : dict[c]) {
            if (cnt == 8)
                fout << byte, byte = 0, cnt = 0;
            if (ch == '1')
                byte |= ((unsigned) 1 << cnt);
            cnt++;
        }
    }
    if (cnt > 0) fout << byte;

    return length_of_extra_information;
}


size_t HuffmanArchiver::extract(std::istream& fin, std::ostream& fout) {
    int32_t n = 0, bits_count = 0;
    fin.read(reinterpret_cast<char*>(&n), sizeof(n));
    if (n == 0) return 0;
    fin.exceptions(std::ifstream::badbit);
    fin.read(reinterpret_cast<char*>(&bits_count), sizeof(int32_t));

    // build HuffmanTree
    HuffmanTree tree{};
    for (int i = 0; i < n; i++) {
        char c;
        std::string s;
        fin.get(c);
        fin >> s; // char and it's code
        tree.add_HuffmanNode(s, (unsigned) c);

        //read space
        fin.get(c);
    }

    const size_t length_of_extra_information = fin.tellg();

    int cnt = 0;

    char ch;
    while (fin.get(ch)) {
        for (unsigned int i = 0; i < 8 && cnt < bits_count; i++) {
            unsigned char c = ((unsigned) ch & ((unsigned) 1 << i)) ? '1' : '0';
            cnt++;

            std::optional<char> res = tree.get_next_char(c);

            if (res.has_value()) fout << res.value();
        }
    }

    return length_of_extra_information;
}
