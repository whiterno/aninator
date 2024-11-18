#ifndef BINARY_HANDBOOK_H
#define BINARY_HANDBOOK_H

//! @brief Node structure, that contains pointers on left, right and parent nodes and a feature
struct Node{
    char* feature;          ///< node's feature
    Node* parent;           ///< pointer on the parent node in tree
    Node* left;             ///< pointer on the left node in tree (its data is less or equal than this node's data)
    Node* right;            ///< pointer on the right node in tree (its data is bigger than this node's data)
};

//! @brief Main structure with debug info, pointer on tree's root and amount of nodes in tree
struct BinDatabase{
    const char* name;       ///< tree's name
    const char* filename;   ///< filename where tree was initialized
    const char* funcname;   ///< funcname where tree was initialized
    size_t line;            ///< serial number of line where tree was initialized

    Node* root;             ///< pointer on tree's root
    int nodes_amount;       ///< amount of nodes in tree
};

enum Errors{
    NO_ERROR,
    NULL_VALUE_INSERTED,
    NO_SUCH_FILE,
};

int playAkinator();

#endif