#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../include/akinator.h"
#include "../include/read_database.h"
#include "../include/utils.h"
#include "../include/big_array.h"
#include "../include/node_utils.h"

static int structRoot(BinDatabase** database_tree, char* database_txt);
static int structDatabase(BinDatabase** database_tree, char** database_txt, Node* parent);
static int structTree(BinDatabase** database_tree, char** database_txt, Node* parent, int tree_dest);
static int nodesDtor(Node* node);

int databaseCtor(const char* db_filename, BinDatabase** database_tree INIT_ARGS_DB){
    assert(db_filename);
    assert(database_tree);

    char* database_txt  = NULL;
    (*database_tree)    = (BinDatabase*)calloc(1, sizeof(BinDatabase));

    INIT_DEBUG_VARS_DB(database_tree);

    readFile(db_filename, &database_txt);
    bigArrayCtor(database_tree);
    structRoot(database_tree, database_txt);

    return NO_ERROR;
}

int readFeature(BinDatabase** database_tree, char* database_txt, char** feature){
    assert(database_tree);
    assert(database_txt);
    assert(feature);

    char* end       = strchr(database_txt + 1, '"');
    int feature_len = end - database_txt;

    if (feature_len > BIG_ARR_ELEM_CAPACITY){
        return FAT_FEATURE;
    }

    addToBigArray(database_tree, database_txt, feature, feature_len);

    return NO_ERROR;
}

int databaseDtor(BinDatabase** database){
    assert(database);

    nodesDtor((*database)->root);
    bigArrayDtor(*database);

    free(*database);
    *database = NULL;

    return NO_ERROR;
}

static int nodesDtor(Node* node){
    assert(node);

    if (node->left != NULL){
        nodesDtor(node->left);
        nodesDtor(node->right);
    }

    node->feature   = NULL;
    node->parent    = NULL;
    node->left      = NULL;
    node->right     = NULL;

    free(node);

    return NO_ERROR;
}

static int structRoot(BinDatabase** database_tree, char* database_txt){
    assert(database_tree);
    assert(database_txt);

    database_txt = strchr(database_txt, '{');
    if (database_txt == NULL || isNodeEmpty(database_txt + 1)) return NO_DATA_BASE;

    database_txt    = strchr(database_txt, '"');
    char* feature   = NULL;
    readFeature(database_tree, database_txt, &feature);
    database_txt    = strchr(database_txt + 1, '"') + 1;

    Node* node = NULL;
    createNode(&node, feature);
    (*database_tree)->root = node;

    (*database_tree)->nodes_amount = 1;

    structDatabase(database_tree, &database_txt, (*database_tree)->root);

    return NO_ERROR;
}

static int structDatabase(BinDatabase** database_tree, char** database_txt, Node* parent){
    assert(database_tree);
    assert(database_txt);
    assert(parent);

    char* open_br  = strchr((*database_txt), '{');
    char* close_br = strchr((*database_txt), '}');

    if (close_br < open_br || open_br == NULL) return NO_ERROR;

    structTree(database_tree, database_txt, parent, LEFT);
    structTree(database_tree, database_txt, parent, RIGHT);

    return NO_ERROR;
}

static int structTree(BinDatabase** database_tree, char** database_txt, Node* parent, int tree_dest){
    assert(database_tree);
    assert(database_txt);
    assert(parent);

    Node* node = NULL;
    fillNode(database_tree, database_txt, &node);
    if (tree_dest == RIGHT) parent->right = node;
    else                    parent->left = node;

    (*database_tree)->nodes_amount++;

    if (tree_dest == RIGHT) structDatabase(database_tree, database_txt, parent->right);
    else                    structDatabase(database_tree, database_txt, parent->left);

    return NO_ERROR;
}
