#include "HuffmanTree.hpp"
#include "PriorityQueue.hpp"
#include <iostream>
#include <fstream>
#include <cctype>
#include "utils.hpp"


#include <map>

//completed implementation of big 5 to guarantee correct behavior

//copy constructor
HuffmanTree::HuffmanTree(const HuffmanTree& other) {
    if (other.root_ == nullptr) {
        root_ = nullptr;
    }
    else {
        root_ = copy(other.root_);
    }
}
//assignment operator
HuffmanTree& HuffmanTree::operator=(const HuffmanTree& other) {
    if (this != &other) {
        destroy(root_);
        if (other.root_ == nullptr) {
            root_ = nullptr;
        }
        else {
            root_ = copy(other.root_);
        }
    }
    return *this;
}

//move constructor
HuffmanTree::HuffmanTree(HuffmanTree&& other) noexcept {
    root_ = other.root_;
    other.root_ = nullptr;
}

//move assignment operator
HuffmanTree& HuffmanTree::operator=(HuffmanTree&& other) noexcept {
    if (this != &other) {
        destroy(root_);
        root_ = other.root_;
        other.root_ = nullptr;
    }
    return *this;
}

//takes in vector of word/frequency pairs, returns a huffman tree
HuffmanTree HuffmanTree::buildFromCounts(const std::vector<std::pair<std::string, int>>& counts) {
    //empty input case
    if (counts.empty()) {
        HuffmanTree emptyTree;
        return emptyTree;
    }

    //Convert each pair to a leaf node and put the nodes in a vector
    std::vector<TreeNode*> leafNodes;
    for (std::size_t i = 0; i < counts.size(); i++) {
        leafNodes.push_back(new TreeNode(counts.at(i).first, counts.at(i).second));
    }

    PriorityQueue queue(leafNodes);

    //Build the tree's internal nodes
    while (queue.size() > 1) {
        TreeNode* left = queue.extractMin();
        TreeNode* right = queue.extractMin();
        
        const int internalNodeFrequency = left->getFrequency() + right->getFrequency();
        std::string internalNodeWord;
        if (left->getWord() < right->getWord()) {
            internalNodeWord = left->getWord();
        }
        else {
            internalNodeWord = right->getWord();
        }

        TreeNode* internalNode = new TreeNode(internalNodeWord, internalNodeFrequency);
        internalNode->setLeft(left);
        internalNode->setRight(right);
        queue.insert(internalNode);
    }

    //return the huffman tree
    TreeNode* rootPointer = queue.extractMin();
    HuffmanTree result;
    result.root_ = rootPointer;
    return result;
}

// destructor
HuffmanTree::~HuffmanTree() {
    destroy(root_);
    root_ = nullptr;
}                         

// Build a vector of (word, code) pairs by traversing the Huffman tree
// (left=0, right=1; visit left before right). 
void HuffmanTree::assignCodes(std::vector<std::pair<std::string, std::string>>& out) const {
    std::string prefix;
    assignCodesDFS(root_, prefix, out);
}

error_type HuffmanTree::writeHeader(std::ostream& os) const {
    if (!os.good())
        return UNABLE_TO_OPEN_FILE_FOR_WRITING;

    std::string prefix;
    writeHeaderPreorder(root_, os, prefix);

    if (os.fail())
        return FAILED_TO_WRITE_FILE;
    return NO_ERROR;
}

// Encode a sequence of tokens using the codebook derived from this tree.
// Writes ASCII '0'/'1' and wraps lines to wrap_cols (80 by default).
error_type HuffmanTree::encode(const std::vector<std::string>& tokens, std::ostream& os_bits, int wrap_cols) const {
    if (root_ == nullptr) {
        if (tokens.empty())
            return NO_ERROR;
        return UNABLE_TO_OPEN_FILE_FOR_WRITING;
    }

    std::vector<std::pair<std::string, std::string>> codePairs;
    assignCodes(codePairs);
    std::map<std::string, std::string> codeMap;

    for (std::size_t i = 0; i < codePairs.size(); i++) {
        codeMap[codePairs.at(i).first] = codePairs.at(i).second;
    }

    if (!os_bits.good())
        return UNABLE_TO_OPEN_FILE_FOR_WRITING;

    std::string currentLine;
    for (std::size_t i = 0; i < tokens.size(); ++i) {
        const std::string& token = tokens.at(i);

        if (codeMap.find(token) == codeMap.end())
            return FAILED_TO_WRITE_FILE;

        const std::string& code = codeMap.at(token);
        for (std::size_t j = 0; j < code.size(); ++j) {
            currentLine.push_back(code.at(j));
            if ((int)currentLine.size() == wrap_cols) {
                os_bits << currentLine << '\n';
                currentLine.clear();
            }
        }
    }

    if (!currentLine.empty())
        os_bits << currentLine << '\n';

    if (os_bits.fail())
        return FAILED_TO_WRITE_FILE;

    return NO_ERROR;
}

TreeNode* HuffmanTree::copy(const TreeNode* n) const {
    if (n == nullptr)
        return nullptr;

    TreeNode* newNode = new TreeNode(n->getWord(), n->getFrequency());
    newNode->setLeft(copy(n->getLeft()));
    newNode->setRight(copy(n->getRight()));
    return newNode;
}

void HuffmanTree::destroy(TreeNode* n) noexcept {
    if (n == nullptr)
        return;

    destroy(n->getLeft());
    destroy(n->getRight());
    delete n;
}

void HuffmanTree::assignCodesDFS(const TreeNode* n, std::string& prefix, std::vector<std::pair<std::string, std::string>>& out) {
    if (n == nullptr)
        return;

    //if the node is a leaf, record its code
    if (n->getLeft() == nullptr && n->getRight() == nullptr) {
        std::string code;
        if (prefix.empty()) {
            code = "0";
        }
        else {
            code = prefix;
        }
        out.push_back(std::make_pair(n->getWord(), code));
        return;
    }

    //left child append 0
    prefix.push_back('0');
    assignCodesDFS(n->getLeft(), prefix, out);
    prefix.pop_back();

    //right child append 1
    prefix.push_back('1');
    assignCodesDFS(n->getRight(), prefix, out);
    prefix.pop_back();
}

void HuffmanTree::writeHeaderPreorder(const TreeNode* n, std::ostream& os, std::string& prefix) {
    if (n == nullptr)
        return;

    if (n->getLeft() == nullptr && n->getRight() == nullptr) {
        std::string code;
        if (prefix.empty()) {
            code = "0";
        }
        else {
            code = prefix;
        }
        os << n->getWord() << " " << code << '\n';
        return;
    }

    prefix.push_back('0');
    writeHeaderPreorder(n->getLeft(), os, prefix);
    prefix.pop_back();

    prefix.push_back('1');
    writeHeaderPreorder(n->getRight(), os, prefix);
    prefix.pop_back();

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
