//Nick Guevara
//CS 315
//10/28/25
//Project 4

//Huffman decoder: reconstructs tree from .hdr and decodes .code into .tokens_decoded

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

#include "utils.hpp"
#include "HuffmanTree.hpp"

namespace fs = std::filesystem;

static error_type readHeader(const fs::path& headerFile, std::vector<std::pair<std::string, std::string>>& pairs);

int main(int argc, char* argv[]) {

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <headerFile> <codeFile>\n";
        return 1;
    }

    fs::path dirPath = "input_output";
    fs::path headerFilePath = dirPath / argv[1];
    fs::path encodedFilePath = dirPath / argv[2];
    fs::path decodedFilePath = dirPath / (encodedFilePath.stem().string() + ".tokens_decoded");

    if (error_type status; (status = directoryExists(dirPath.string())) != NO_ERROR)
        exitOnError(status, dirPath.string());
    if (error_type status; (status = regularFileExistsAndIsAvailable(headerFilePath.string())) != NO_ERROR)
        exitOnError(status, headerFilePath.string());
    if (error_type status; (status = regularFileExistsAndIsAvailable(encodedFilePath.string())) != NO_ERROR)
        exitOnError(status, encodedFilePath.string());
    if (error_type status; (status = canOpenForWriting(decodedFilePath.string())) != NO_ERROR)
        exitOnError(status, encodedFilePath.string());

    // Part 4: Read the header, build the huffman tree, write the encoded file

    std::vector<std::pair<std::string, std::string>> codePairs;
    if (error_type status = readHeader(headerFilePath, codePairs); status != NO_ERROR)
        exitOnError(status, headerFilePath.string());

    HuffmanTree decodingTree;
    if (error_type status; (status = decodingTree.buildFromHeader(codePairs)) != NO_ERROR)
        exitOnError(status, headerFilePath.string());
    if (error_type status; (status = decodingTree.decode(encodedFilePath, decodedFilePath)) != NO_ERROR)
        exitOnError(status, decodedFilePath.string());
  
    return 0;
}

static error_type readHeader(const fs::path& headerFile, std::vector<std::pair<std::string, std::string>>& pairs) {
    std::ifstream inFile(headerFile);
    if (!inFile.is_open())
        return UNABLE_TO_OPEN_FILE;

    char firstChar = inFile.peek();
    if (!inFile || !std::isalpha(static_cast<unsigned char>(firstChar))) {
        std::cerr << "Error: Header file does not start with a letter.\n";
        return FAILED_TO_WRITE_FILE; // Using FAILED_TO_WRITE_FILE to represent invalid format 
    }

    std::string word, code;
    while (inFile >> word >> code)
        pairs.emplace_back(word, code);

    inFile.close();
    return NO_ERROR;
}