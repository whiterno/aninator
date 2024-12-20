#include <stdio.h>
#include <assert.h>

#include "../include/akinator.h"
#include "../include/stack_utils.h"
#include "../include/utils.h"
#include "../include/node_utils.h"
#include "../include/definition_mode.h"
#include "../include/difference_mode.h"

#include "../lib/stack/stack_hcpp/stack.h"

static int printDifferences(BinDatabase* database, Stack* path1, const char* object1, Stack* path2, const char* object2);

int tellDifference(BinDatabase* database){
    assert(database);

    char object1[BIG_ARR_ELEM_CAPACITY] = {};
    char object2[BIG_ARR_ELEM_CAPACITY] = {};
    printf("Какие объекты вы хотите сравнить?\n");

    printf("Объект 1: ");
    scanf("%s", object1);
    clearBuffer();

    printf("Объект 2: ");
    scanf("%s", object2);
    clearBuffer();

    Stack* path1 = stackCtor(INIT(path1) 0);
    if (findPath(database, object1, path1) == NO_OBJECT_IN_DB) return NO_OBJECT_IN_DB;

    Stack* path2 = stackCtor(INIT(path2) 0);
    if (findPath(database, object2, path2) == NO_OBJECT_IN_DB) return NO_OBJECT_IN_DB;

    printDifferences(database, path1, object1, path2, object2);
    stackDtor(&path1);
    stackDtor(&path2);

    return NO_ERROR;
}

static int printDifferences(BinDatabase* database, Stack* path1, const char* object1, Stack* path2, const char* object2){
    assert(database);
    assert(path1);
    assert(object1);
    assert(path2);
    assert(object2);

    reverseStack(path1);
    reverseStack(path2);

    printf("%s и %s оба:\n", object1, object2);

    int step1 = 1;
    int step2 = 2;
    Node* ptr1 = database->root;
    Node* ptr2 = database->root;
    while (true){
        if (stackPop(path1, &step1) == STACK_UNDERFLOW) break;
        stackPop(path2, &step2);

        if (step1 != step2) break;
        if (step1 == 0){
            printf("не %s, ", ptr1->feature);
            ptr1 = ptr1->right;
            ptr2 = ptr2->right;
        }
        else{
            printf("%s, ", ptr1->feature);
            ptr1 = ptr1->left;
            ptr2 = ptr2->left;
        }
    }

    printf("\nНо:\n");
    stackPush(path1, step1);
    stackPush(path2, step2);
    reverseStack(path1);
    reverseStack(path2);
    printObjectFeatures(ptr1, path1, object1);
    printObjectFeatures(ptr2, path2, object2);

    return NO_ERROR;
}
