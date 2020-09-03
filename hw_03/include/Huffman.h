//
// Created by mark on 4/18/20.
//

#ifndef HW_03_HUFFMAN_H
#define HW_03_HUFFMAN_H

#include <queue>
#include <cstddef>
#include <unordered_map>
#include <memory>


class HuffmanException final : public std::exception {
public:
    explicit HuffmanException(std::string msg) : msg_(std::move(msg)) {}

    [[nodiscard]] const char* what() const noexcept override;

private:
    std::string msg_;
};

class HuffmanTree final {
private:
    class HuffmanNode final {
    public:
        size_t freq = 0;
        unsigned char data = 0;
        std::unique_ptr<HuffmanNode> left0;
        std::unique_ptr<HuffmanNode> right1;

        class compare_by_frequency {
        public:
            bool operator()(HuffmanNode* a, HuffmanNode* b) noexcept {
                return a->freq > b->freq;
            }
        };

    };

    using my_queue = std::priority_queue<HuffmanNode*,
            std::vector<HuffmanNode*>, HuffmanTree::HuffmanNode::compare_by_frequency>;

    void generate_codes_(const HuffmanNode* node, std::unordered_map<char, std::string>& dict,
                         int32_t& freq_sum, const std::string& code = "");


    std::unique_ptr<HuffmanNode> top;
    HuffmanNode* cur = nullptr;
public:

    HuffmanTree() = default;

    explicit HuffmanTree(my_queue HuffmanNode_queue);

    static my_queue priority_queue_with_HuffmanNodes(
            const std::unordered_map<char, int>& letters);

    int32_t generate_codes(std::unordered_map<char, std::string>& dict);

    static int32_t zip(std::unordered_map<char, int>& letters, std::unordered_map<char, std::string>& dict);

    void add_HuffmanNode(const std::string& s, unsigned char ch);

    std::optional<char> get_next_char(unsigned char c);

    friend class TestClass;

};


#endif //HW_03_HUFFMAN_H
