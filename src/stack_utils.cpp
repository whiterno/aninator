#include <assert.h>

#include "../include/stack_utils.h"

#include "../lib/stack/stack_hcpp/stack.h"

// TODO: удалить это дерьмо нахуй (но оно работает)

void reverseStack(Stack* stk){
    assert(stk);

    Stack* tmp1 = stackCtor(INIT(tmp1) 0);
    Stack* tmp2 = stackCtor(INIT(tmp2) 0);

    fromStkToStk(stk, tmp1);
    fromStkToStk(tmp1, tmp2);
    fromStkToStk(tmp2, stk);

    stackDtor(&tmp1);
    stackDtor(&tmp2);
}

void fromStkToStk(Stack* stk1, Stack* stk2){
    assert(stk1);
    assert(stk2);

    while (true){
        int tmp = 0;

        if (stackPop(stk1, &tmp) == 4){
            break;
        }
        stackPush(stk2, tmp);
    }
}
