#include <assert.h>
#include <stdio.h>

#include "../include/stack_utils.h"

#include "../lib/stack/stack_hcpp/stack.h"

void reverseStack(Stack* stk){
    assert(stk);

    int tmp = 0;
    for (int i = 0; i < (stk->size) / 2; i++){
        tmp = stk->data[i];
        stk->data[i] = stk->data[stk->size - 1 - i];
        stk->data[stk->size - 1 - i] = tmp;
    }
}

void printStack(Stack* stk){
    assert(stk);

    for (int i = 0; i < stk->size; i++){
        printf("%d ", stk->data[i]);
    }
    printf("\n");
}
