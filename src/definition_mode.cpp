#include <stdio.h>
#include <assert.h>

#include "../include/akinator.h"
#include "../include/definition_mode.h"
#include "../include/utils.h"
#include "../include/node_utils.h"
#include "../include/stack_utils.h"

#include "../lib/stack/stack_hcpp/stack.h"

int tellDefinition(BinDatabase* database){
    assert(database);

    char object[BIG_ARR_ELEM_CAPACITY] = {};
    printf("Что за объект вы хотите найти?\n");
    scanf("%s", object);
    clearBuffer();

    Node* required_node = NULL;
    Stack* path = stackCtor(INIT(PATH) 0);
    findNode(database->root, object, &required_node, path);

    if (required_node == NULL){
        printf("Нет такого объекта в базе данных!\n");

        return NO_OBJECT_IN_DB;
    }

    printObjectFeatures(database->root, path, object);
    stackDtor(&path);

    return NO_ERROR;
}

int printObjectFeatures(Node* node, Stack* path, char* object){
    assert(node);
    assert(path);
    assert(object);

    printf("%s ", object);

    reverseStack(path);
    int step = 0;
    Node* ptr = node;
    while(stackPop(path, &step) != 4){
        if (step == 0){
            printf("не %s, ", ptr->feature);
            ptr = ptr->right;
        }
        else{
            printf("%s, ", ptr->feature);
            ptr = ptr->left;
        }
    }
    printf("\n");

    return NO_ERROR;
}
