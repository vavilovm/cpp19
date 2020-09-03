//
// Created by mark on 4/18/20.
//

#ifndef HW_03_HUFFMANARCHIVER_H
#define HW_03_HUFFMANARCHIVER_H

#include <iostream>
#include <fstream>
#include "Huffman.h"

class HuffmanArchiver {
public:
    static size_t compress(std::istream& fin, std::ostream& fout);

    static size_t extract(std::istream& fin, std::ostream& fout);
};


#endif //HW_03_HUFFMANARCHIVER_H
