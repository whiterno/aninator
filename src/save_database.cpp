#include <stdio.h>
#include <assert.h>

#include "../include/akinator.h"
#include "../include/save_database.h"
#include "../include/utils.h"

static int writeDatabase(FILE* fp, Node* node, const int tabs);

int saveDatabase(const char* filename, BinDatabase* database){
    assert(filename);
    assert(database);

    FILE* fp = fopen(filename, "w");
    if (fp == NULL) return FILE_NOT_OPEN;

    writeDatabase(fp, database->root, 0);

    return NO_ERROR;
}

static int writeDatabase(FILE* fp, Node* node, const int tabs){
    assert(fp);
    assert(node);

    fprintfnTab(fp, tabs);
    fprintf(fp, "{ \"%s\" ", node->feature);

    if (node->left != NULL){
        fprintf(fp, "\n");
        writeDatabase(fp, node->left, tabs + 1);
        writeDatabase(fp, node->right, tabs + 1);
        fprintfnTab(fp, tabs);
        fprintf(fp, "}\n");
    }
    else{
        fprintf(fp, "}\n");
    }

    return NO_ERROR;
}
