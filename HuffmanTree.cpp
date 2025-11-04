#include "HuffmanTree.hpp"
#include <iostream>
#include <fstream>
#include "utils.hpp"

// destructor
HuffmanTree::~HuffmanTree() {
    destroy(root_);
    root_ = nullptr;
}                         

void HuffmanTree::destroy(TreeNode* n) noexcept {
    if (n == nullptr)
        return;

    destroy(n->getLeft());
    destroy(n->getRight());
    delete n;
}

//initialize an empty huffman tree with the contents of a header file
//build internal nodes until a leaf is reached, then build leaf
error_type HuffmanTree::buildFromHeader(const std::vector<std::pair<std::string, std::string>>& pairs) {
    destroy(root_);
    root_ = new TreeNode("", 0);

    for (std::size_t i = 0; i < pairs.size(); i++) {
        const std::string& word = pairs.at(i).first;
        const std::string& code = pairs.at(i).second;

        TreeNode* current = root_;
        for (std::size_t j = 0; j < code.size(); j++) {
            char bit = code.at(j);

            if (bit == '0') {
                if (current->getLeft() == nullptr)
                    current->setLeft(new TreeNode("", 0));
                current = current->getLeft();
            }
            else if (bit == '1') {
                if (current->getRight() == nullptr)
                    current->setRight(new TreeNode("", 0));
                current = current->getRight();
            }
            else {
                continue;
            }
        }

        current->setWord(word);
    }

    return NO_ERROR;
}

//traverses the huffman tree according to the bit sequence in the encoded file.
//finds the encoded words at the leaf nodes and outputs them to the decoded file. 
error_type HuffmanTree::decode(const fs::path& encodedFilePath, const fs::path& decodedFilePath) {
    std::ifstream inFile(encodedFilePath);
    if (!inFile.is_open())
        return UNABLE_TO_OPEN_FILE;
    char firstChar = inFile.peek();
    if (firstChar != '0' && firstChar != '1') {
        std::cerr << "Error: Code file does not start with 0 or 1.\n";
        return FAILED_TO_WRITE_FILE;
    }

    std::ofstream outFile(decodedFilePath, std::ios::out | std::ios::trunc);
    if (!outFile.is_open())
        return UNABLE_TO_OPEN_FILE_FOR_WRITING;

    TreeNode* current = root_;
    char bit;

    while (inFile.get(bit)) {
        if (bit == '0') {
            current = current->getLeft();
        }
        else if (bit == '1') {
            current = current->getRight();
        }
        else if (bit == '\n' || bit == '\r'){
            continue;
        }
        else {
            continue;
        }

        //if reached a leaf node, output the encoded word and a newline
        if (current->getLeft() == nullptr && current->getRight() == nullptr){
            outFile << current->getWord() << '\n';
            current = root_;
        }
    }

    if (current != root_ && (current->getLeft() != nullptr || current->getRight() != nullptr)) {
        std::cerr << "Error: Encoded file ended before reaching a leaf node.\n";
        inFile.close();
        outFile.close();
        return FAILED_TO_WRITE_FILE;
    }
    
    inFile.close();
    outFile.close();
    return NO_ERROR;

}
