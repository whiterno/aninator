#ifndef AKINATOR_H
#define AKINATOR_H

#define INIT_DB(binDatabase) , #binDatabase, __FILE__, __func__, __LINE__
#define INIT_ARGS_DB , const char* name, const char* filename, const char* funcname, size_t line
#define INIT_DEBUG_VARS_DB(binDatabase) (*binDatabase)->name = name; (*binDatabase)->filename = filename; (*binDatabase)->funcname = funcname; (*binDatabase)->line = line;
#define DUMP_DB(binDatabase) binDatabase, __FILE__, __func__, __LINE__

const int BIG_ARR_ELEM_CAPACITY = 128;
const int BIG_ARR_BEG_CAPACITY  = 4;

//! @brief Node structure, that contains pointers on left, right and parent nodes and a feature
struct Node{
    char* feature;          ///< node's feature
    Node* parent;           ///< pointer on the parent node in tree
    Node* left;             ///< pointer on the left node in tree (its data is less or equal than this node's data)
    Node* right;            ///< pointer on the right node in tree (its data is bigger than this node's data)
};

struct BigArrElem{
    char* beg;
    char* end;
};

//! @brief Main structure with debug info, pointer on tree's root and amount of nodes in tree
struct BinDatabase{
    const char* name;       ///< tree's name
    const char* filename;   ///< filename where tree was initialized
    const char* funcname;   ///< funcname where tree was initialized
    size_t line;            ///< serial number of line where tree was initialized

    Node* root;             ///< pointer on tree's root
    BigArrElem* big_array;
    size_t big_array_capacity;
    int nodes_amount;       ///< amount of nodes in tree
};

enum ErrorsDB{
    NO_ERROR,
    NULL_VALUE_INSERTED,
    NO_SUCH_FILE,
    NO_DATA_BASE,
    FAT_FEATURE,
    FILE_NOT_OPEN,
    CYCLE_IN_TREE,
    UNKNOWN_MODE,
    ADD_NODE,
    NO_OBJECT_IN_DB,
};

enum Emptiness{
    NOT_EMPTY,
    EMPTY,
};

enum Mode{
    GUESS,
    DEFINITION,
    DIFFERENCE,
    DUMP,
};

int playAkinator(const char* db_filename);

#endif
