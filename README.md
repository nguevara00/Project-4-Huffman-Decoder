# Project 3: Huffman Encoding (Part 2 - BST and Priority Queue)
# Project 4: Huffman Decoding

## Student Information
**Name:** Nicholas Guevara  
**Student ID:** 002-85-1971
**Repository:** https://github.com/nguevara00/Project-4-Huffman-Decoder/

## Collaboration & Sources

This project is my own work and I wrote the code myself, based on project 3.

## Implementation Details

- **Design** 

    -  Project 4 extends the Huffman Tree program to decode compressed files.
    -  A huffman tree is reconstructed using word/code pairs from a .hdr file. 
    -  The program traverses the tree according to the sequence in the .code file to recreate the original encoded text.
    -  Error handling and file handling scaffolding reused from project 3.
	
- **Data Structures**
    - **'Huffman Tree'** - Stores nodes that represent either a word(leaf nodes) or internal nodes. 
	- Reconstructed from .hdr using preorder traversasl. 
    - **`TreeNode`** – Basic node structure holding a word, frequency, and left/right pointers.

## Testing & Status

- **Status** Program successfully compiles, runs, and produces expected outputs.
- **Testing** Using the provided scripts on blue returned 11 matches and 0 diffs.
- **Known Issues** None.