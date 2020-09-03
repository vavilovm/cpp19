//
// Created by mark on 4/20/20.
//

#ifndef HW_03_UNITTESTS_H
#define HW_03_UNITTESTS_H

#include <iostream>
#include <doctest/doctest.h>
#include <sstream>
#include "Huffman.h"


class TestClass {
public:
    static void test_build_queue() {
        std::unordered_map<char, int> letters;

        letters['a'] = 100, letters['b'] = 200;
        auto queue = HuffmanTree::priority_queue_with_HuffmanNodes(letters);
                CHECK(queue.size() == 2);
                CHECK(queue.top()->data == 'a');
                CHECK(queue.top()->freq == 100);
        delete queue.top();
        queue.pop();

                CHECK(queue.top()->data == 'b');
                CHECK(queue.top()->freq == 200);
        delete queue.top();
        queue.pop();
    }

    static void test_build_unzip_tree() {
        HuffmanTree tree;

        tree.add_HuffmanNode("00", 'a');
        tree.add_HuffmanNode("01", 'b');
        tree.add_HuffmanNode("110", 'c');
                CHECK(tree.top->left0->left0->data == 'a');
                CHECK(tree.top->left0->right1->data == 'b');
                CHECK(tree.top->right1->right1->left0->data == 'c');
                CHECK(tree.top->left0->left0->left0 == nullptr);
                CHECK(tree.top->right1->right1->right1 == nullptr);
    }

    static void test_get_next_char() {
        // init tree
        HuffmanTree tree;
        tree.add_HuffmanNode("00", 'a');
        tree.add_HuffmanNode("01", 'b');

        std::optional<char> res = std::nullopt;
        res = tree.get_next_char('0');
                CHECK(!res.has_value());
        res = tree.get_next_char('0');
                CHECK(res.value() == 'a');

        res = tree.get_next_char('0');
                CHECK(!res.has_value());
        res = tree.get_next_char('1');
                CHECK(res.value() == 'b');

    }

    static void test_build_tree_with_queue() {
        std::unordered_map<char, int> letters;
        letters['a'] = 100, letters['b'] = 200;
        auto queue = HuffmanTree::priority_queue_with_HuffmanNodes(letters);

        HuffmanTree tree(queue);
                CHECK(tree.top->freq == 300);
                CHECK(tree.top->left0->data == 'a');
                CHECK(tree.top->right1->data == 'b');
    }

    static void test_gen_codes() {
        std::unordered_map<char, int> letters;
        letters['a'] = 128, letters['b'] = 256;
        auto queue = HuffmanTree::priority_queue_with_HuffmanNodes(letters);
        HuffmanTree tree(queue);
        std::unordered_map<char, std::string> dict;

        // total number
                CHECK(tree.generate_codes(dict) == 128 / 8 + 256 / 8);
                CHECK(dict['a'] == "0");
                CHECK(dict['b'] == "1");
    }
};

TEST_CASE ("build queue") {
    TestClass::test_build_queue();
}

TEST_CASE ("build unzip tree") {
    TestClass::test_build_unzip_tree();
}

TEST_CASE ("get_next_char") {
    TestClass::test_get_next_char();
}

TEST_CASE ("Tree(queue) build test") {
    TestClass::test_build_tree_with_queue();
}


#endif //HW_03_UNITTESTS_H
