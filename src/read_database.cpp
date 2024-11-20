#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../include/akinator.h"
#include "../include/read_database.h"
#include "../include/utils.h"
#include "../include/big_array.h"
#include "../include/node_utils.h"

int databaseCtor(const char* db_filename, BinDatabase** database_tree INIT_ARGS_DB){
    assert(db_filename);
    assert(database_tree);

    char* database_txt = NULL;
    (*database_tree) = (BinDatabase*)calloc(1, sizeof(BinDatabase));

    INIT_DEBUG_VARS_DB(database_tree);

    readFile(db_filename, &database_txt);
    bigArrayCtor(database_tree);
    structRoot(database_tree, database_txt);

    return NO_ERROR;
}

int structRoot(BinDatabase** database_tree, char* database_txt){
    assert(database_tree);
    assert(database_txt);

    database_txt = strchr(database_txt, '{');
    if (database_txt == NULL || isNodeEmpty(database_txt + 1)) return NO_DATA_BASE;

    database_txt = strchr(database_txt, '"');
    char* feature = NULL;
    readFeature(database_tree, database_txt, &feature);
    database_txt = strchr(database_txt + 1, '"') + 1;

    Node* node = NULL;
    createNode(&node, feature);
    (*database_tree)->root = node;

    (*database_tree)->nodes_amount = 1;

    structDatabase(database_tree, &database_txt, (*database_tree)->root);

    return NO_ERROR;
}

int structDatabase(BinDatabase** database_tree, char** database_txt, Node* parent){
    assert(database_tree);
    assert(database_txt);
    assert(parent);

    char* open_br  = strchr((*database_txt), '{');
    char* close_br = strchr((*database_txt), '}');

    if (close_br < open_br || open_br == NULL) return NO_ERROR;

    structLeftTree(database_tree, database_txt, parent);
    structRightTree(database_tree, database_txt, parent);

    return NO_ERROR;
}

int readFeature(BinDatabase** database_tree, char* database_txt, char** feature){
    assert(database_tree);
    assert(database_txt);
    assert(feature);

    char* end = strchr(database_txt + 1, '"');
    int feature_len = end - database_txt;

    if (feature_len > BIG_ARR_ELEM_CAPACITY){
        return FAT_FEATURE;
    }

    addToBigArray(database_tree, database_txt, feature, feature_len);

    return NO_ERROR;
}

int structLeftTree(BinDatabase** database_tree, char** database_txt, Node* parent){
    assert(database_tree);
    assert(database_txt);
    assert(parent);

    Node* node = NULL;
    fillNode(database_tree, database_txt, &node);
    parent->left = node;

    (*database_tree)->nodes_amount++;

    structDatabase(database_tree, database_txt, parent->left);

    return NO_ERROR;
}

int structRightTree(BinDatabase** database_tree, char** database_txt, Node* parent){
    assert(database_tree);
    assert(database_txt);
    assert(parent);

    Node* node = NULL;
    fillNode(database_tree, database_txt, &node);
    parent->right = node;

    (*database_tree)->nodes_amount++;

    structDatabase(database_tree, database_txt, parent->right);

    return NO_ERROR;
}
