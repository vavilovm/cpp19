//
// Created by mark on 4/18/20.
//
#include <unordered_map>
#include <vector>
#include <queue>
#include <fstream>
#include <cassert>
#include <iostream>
#include "Huffman.h"


// join top HuffmanNode_queue's
HuffmanTree::HuffmanTree(my_queue HuffmanNode_queue) {

    std::unique_ptr<HuffmanTree::HuffmanNode> new_parent_HuffmanNode;

    while (!HuffmanNode_queue.empty()) {

        // create parent HuffmanNode
        new_parent_HuffmanNode = std::make_unique<HuffmanTree::HuffmanNode>();

        // add left HuffmanNode
        new_parent_HuffmanNode->left0 = std::unique_ptr<HuffmanTree::HuffmanNode>(HuffmanNode_queue.top());
        new_parent_HuffmanNode->freq += new_parent_HuffmanNode->left0->freq;
        HuffmanNode_queue.pop();

        // add right HuffmanNode
        if (!HuffmanNode_queue.empty()) {
            new_parent_HuffmanNode->right1 = std::unique_ptr<HuffmanTree::HuffmanNode>(HuffmanNode_queue.top());
            new_parent_HuffmanNode->freq += new_parent_HuffmanNode->right1->freq;
            HuffmanNode_queue.pop();

            // push new HuffmanNode
            if (!HuffmanNode_queue.empty()) {
                HuffmanNode_queue.push(new_parent_HuffmanNode.release());
            }
        }
    }
    top = std::move(new_parent_HuffmanNode);
}

HuffmanTree::my_queue HuffmanTree::priority_queue_with_HuffmanNodes(const std::unordered_map<char, int>& letters) {
    HuffmanTree::my_queue result;

    for (const auto c : letters) {
        auto* node = new HuffmanTree::HuffmanNode;
        node->freq = c.second;
        node->data = c.first;
        result.push(node);
    }
    return result;
}


// generates codes + counts sum length
void HuffmanTree::generate_codes_(const HuffmanNode* node, std::unordered_map<char, std::string>& dict,
                                  int32_t& freq_sum, const std::string& code) {
    if (!node) return;
    if (node->left0) {
        generate_codes_(node->left0.get(), dict, freq_sum, code + '0');
        generate_codes_(node->right1.get(), dict, freq_sum, code + '1');
    } else {
        if (!code.empty()) {
            dict[node->data] = code;
            freq_sum += node->freq * code.size();
        }
    }
}

int32_t HuffmanTree::zip(std::unordered_map<char, int>& letters, std::unordered_map<char, std::string>& dict) {
    HuffmanTree tree(priority_queue_with_HuffmanNodes(letters));
    int32_t size_of_result = tree.generate_codes(dict);
    return size_of_result;
}


void HuffmanTree::add_HuffmanNode(const std::string& s, unsigned char ch) {
    if (!top) top = std::make_unique<HuffmanNode>();
    cur = top.get();

    for (auto c : s) {
        if (c == '0') { // 0 -- go left
            if (!cur->left0) {
                cur->left0 = std::make_unique<HuffmanNode>();
            }
            cur = cur->left0.get();
        } else {
            if (!cur->right1) {
                cur->right1 = std::make_unique<HuffmanNode>();
            }
            cur = cur->right1.get();
        }
    }
    cur->data = ch;
    cur = top.get();
}

std::optional<char> HuffmanTree::get_next_char(const unsigned char c) {
    if (!cur || (!cur->left0 && !cur->right1)) {
        throw HuffmanException("Wrong position of current ptr. Tree is not built or is built wrong.");
    }

    std::optional<char> ans = std::nullopt;
    cur = (c == '0') ? cur->left0.get() : cur->right1.get();
    if (!cur->left0) {
        ans = cur->data, cur = top.get();
    }
    return ans;
}

int32_t HuffmanTree::generate_codes(std::unordered_map<char, std::string>& dict) {
    int size_of_result = 0;
    generate_codes_(top.get(), dict, size_of_result);
    return size_of_result;
}

const char* HuffmanException::what() const noexcept {
    return msg_.c_str();
}
