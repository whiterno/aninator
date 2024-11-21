#include <stdio.h>
#include <assert.h>

#include "../include/akinator.h"
#include "../include/guess_mode.h"
#include "../include/node_utils.h"
#include "../include/utils.h"
#include "../include/save_database.h"

static int getAns();

int playGuess(BinDatabase* database, Node* node){
    assert(node);
    assert(database);

    if (node->left != NULL){
        printf("%s ? [y/n]\n", node->feature);
        if (getAns()){
            playGuess(database, node->left);
        }
        else{
            playGuess(database, node->right);
        }

        return NO_ERROR;
    }

    printf("Это: %s ? [y/n]\n", node->feature);
    if (getAns()){
        printf("Я умнее тебя, у меня памяти 16 мегабайт\n");

        return NO_ERROR;
    }

    addNode(database, node);
    printf("Хотите сохранить базу данных? [y/n]\n");
    if (getAns()){
        saveDatabase("database/saved_db.db", database);
    }
    return NO_ERROR;
}

static int getAns(){
    int character = getchar();

    while (true){
        if (character == 'y'){
            clearBuffer();
            return 1;
        }
        if (character == 'n'){
            clearBuffer();
            return 0;
        }

        printf("Нет таких вариантов ответа");
        clearBuffer();
        character = getchar();
    }
}
