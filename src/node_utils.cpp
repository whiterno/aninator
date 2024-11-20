#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "../include/akinator.h"
#include "../include/node_utils.h"
#include "../include/utils.h"
#include "../include/read_database.h"

#include "../lib/stack/stack_hcpp/stack.h"

int createNode(Node** node, char* feature){
    assert(feature);
    if (node == NULL){
        return NULL_VALUE_INSERTED;
    }

    *node = (Node*)calloc(1, sizeof(Node));

    (*node)->left = NULL;
    (*node)->right = NULL;
    (*node)->parent = NULL;

    (*node)->feature = feature;

    return NO_ERROR;
}

int fillNode(BinDatabase** database_tree, char** database_txt, Node** node){
    assert(database_tree);
    assert(database_txt);
    assert(node);

    (*database_txt) = strchr((*database_txt), '"');
    char* feature = NULL;
    readFeature(database_tree, *database_txt, &feature);
    (*database_txt) = strchr((*database_txt) + 1, '"') + 1;

    createNode(node, feature);

    return NO_ERROR;
}

int isNodeEmpty(char* begin){
    char* end = strchr(begin, '}');

    while (begin != end){
        if (*begin != ' '){
            return NOT_EMPTY;
        }
        begin++;
    }

    return EMPTY;
}

int findNode(Node* node, char* object, Node** required_node, Stack* path){
    assert(node);
    assert(object);
    assert(required_node);
    assert(path);

    if ((*required_node) != NULL) return NO_ERROR;

    if (node->left == NULL){
        if (strcmp(object, node->feature) == 0){
            (*required_node) = node;
        }

        return NO_ERROR;
    }

    stackPush(path, 1);
    findNode(node->left, object, required_node, path);
    if ((*required_node) != NULL) return NO_ERROR;
    int tmp = 0;
    stackPop(path, &tmp);

    stackPush(path, 0);
    findNode(node->right, object, required_node, path);
    if ((*required_node) != NULL) return NO_ERROR;
    stackPop(path, &tmp);

    return NO_ERROR;
}

int addNode(BinDatabase* database, Node* node){
    assert(database);

    char object[BIG_ARR_ELEM_CAPACITY] = {};
    char object_shell[BIG_ARR_ELEM_CAPACITY + 1] = {};
    char feature[BIG_ARR_ELEM_CAPACITY] = {};
    char feature_shell[BIG_ARR_ELEM_CAPACITY + 1] = {};

    printf("Кого вы загадали?\n");
    scanf("%s", object);
    printf("Чем %s отличиается от %s ?\n", object, node->feature);
    scanf("%s", feature);
    clearBuffer();

    object_shell[0] = '"';
    strcpy(object_shell + 1, object);
    object_shell[strlen(object_shell)] = '"';

    feature_shell[0] = '"';
    strcpy(feature_shell + 1, feature);
    feature_shell[strlen(feature_shell)] = '"';

    char* object_ptr = NULL;
    char* feature_ptr = NULL;
    readFeature(&database, object_shell, &object_ptr);
    readFeature(&database, feature_shell, &feature_ptr);

    Node* object_node = NULL;
    createNode(&object_node, object_ptr);
    Node* old_node = NULL;
    createNode(&old_node, node->feature);

    node->feature = feature_ptr;
    node->left = object_node;
    node->right = old_node;
    database->nodes_amount += 2;

    return NO_ERROR;
}
