#ifndef HUFFMAN_TREE_HPP
#define HUFFMAN_TREE_HPP


#include "TreeNode.hpp"
#include "utils.hpp"
#include <vector>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class HuffmanTree {
public:
    HuffmanTree() = default;
    ~HuffmanTree();
    error_type buildFromHeader(const std::vector<std::pair<std::string, std::string>>& pairs);
    error_type decode(const fs::path& code, const fs::path& decodedFile);

private:
    TreeNode* root_ = nullptr;
    static void destroy(TreeNode* n) noexcept;
};

#endif // HUFFMAN_TREE_HPP
