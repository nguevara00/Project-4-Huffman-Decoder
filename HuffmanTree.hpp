#ifndef HUFFMAN_TREE_HPP
#define HUFFMAN_TREE_HPP

#include <vector>
#include <filesystem>
#include "TreeNode.hpp"
#include "utils.hpp"
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
    TreeNode* copy(const TreeNode* n) const;
};

#endif // HUFFMAN_TREE_HPP
