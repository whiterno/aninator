#ifndef AKINATOR_H
#define AKINATOR_H

#define INIT_DB(binDatabase) , #binDatabase, __FILE__, __func__, __LINE__
#define INIT_ARGS_DB , const char* name, const char* filename, const char* funcname, size_t line
#define INIT_DEBUG_VARS_DB(binDatabase) (*binDatabase)->name = name; (*binDatabase)->filename = filename; (*binDatabase)->funcname = funcname; (*binDatabase)->line = line;
#define DUMP_DB(binDatabase) binDatabase, __FILE__, __func__, __LINE__

//! @brief Size of allocated memory (in bytes) for one big array element
const int BIG_ARR_ELEM_CAPACITY = 128;

//! @brief The original big array capacity
const int BIG_ARR_BEG_CAPACITY  = 8;

//! @brief Node structure, that contains pointers on left, right and parent nodes and a feature
struct Node{
    char* feature;          ///< node's feature
    Node* parent;           ///< pointer on the parent node in tree
    Node* left;             ///< pointer on the left node in tree (its data is less or equal than this node's data)
    Node* right;            ///< pointer on the right node in tree (its data is bigger than this node's data)
};

//! @brief Structure of a big array element, that contains 2 pointers on allocated memory
struct BigArrElem{
    char* beg;              ///< pointer on the begginning of allocated memory (for free)
    char* end;              ///< pointer on the last occupied byte
};

//! @brief Main structure with debug info, pointer on tree's root and amount of nodes in tree
struct BinDatabase{
    const char* name;           ///< tree's name
    const char* filename;       ///< filename where tree was initialized
    const char* funcname;       ///< funcname where tree was initialized
    size_t line;                ///< serial number of line where tree was initialized

    Node* root;                 ///< pointer on tree's root
    BigArrElem* big_array;      ///< array that contains pointers on allocated memory for features
    size_t big_array_capacity;  ///< amount of cells (sizeof array / sizeof elem) in big array
    int nodes_amount;           ///< amount of nodes in tree
};

//! @brief Database errors (and Akinator)
enum ErrorsDB{
    NO_ERROR,                   ///< No error occured
    NULL_VALUE_INSERTED,        ///< NULL value was given as an argument in inappropriate place
    NO_DATA_BASE,               ///< Programm couldn't find database
    FAT_FEATURE,                ///< Feature is too long to be contained in memory
    FILE_NOT_OPEN,              ///< File was not opened due to some mistake (no such file, wrong path etc)
    CYCLE_IN_TREE,              ///< Cycle in tree occured
    UNKNOWN_MODE,               ///< Akinator couldn't process the give mode
    ADD_NODE,                   ///< New node is needed in DB
    NO_OBJECT_IN_DB,            ///< Object was not found in database
};

//! @brief Stack (lib data structure) errors
enum ErrorsStack{
    STACK_UNDERFLOW = 4,        ///< StackPop couldn't pop element because there are no elements in stack
};

//! @brief Node's emptiness
enum Emptiness{
    NOT_EMPTY,      ///< some feature is written in the node
    EMPTY,          ///< empty node, no feature is written
};

//! @brief The destination of tree's recursive traversal
enum TreeDest{
    RIGHT,          ///< go into the right subtree
    LEFT,           ///< go into the left subtree
};

//! @brief Play modes for 'Akinator'
enum Mode{
    GUESS,          ///< guess an object and add to database if wasn't found in it
    DEFINITION,     ///< tell the definition of an object written in database
    DIFFERENCE,     ///< tell the difference between two objects of database
    DUMP,           ///< make a graphic dump of database (generated a graph with nodes, edges and other info)
};


//!
//! @brief Function that starts the 'Akinator' game with a few modes
//!
//! @param [in] db_filename     the name of file where database for akinator is
//!
//! @return NO_ERROR if everything is fine and UNKNOWN_MODE if an error in modes occured
//!
int playAkinator(const char* db_filename);

#endif
