#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../include/akinator.h"
#include "../include/akinator_dump.h"
#include "../include/read_database.h"
#include "../include/utils.h"
#include "../include/guess_mode.h"
#include "../include/difference_mode.h"
#include "../include/definition_mode.h"

#include "../lib/stack/stack_hcpp/stack.h"

static void printIntro();
static int getMode();
static int processMode(const int mode, BinDatabase* database);

int playAkinator(const char* db_filename){
    assert(db_filename);

    BinDatabase* db = NULL;
    databaseCtor(db_filename, &db INIT_DB(db));

    printIntro();
    processMode(getMode(), db);

    return NO_ERROR;
}

static void printIntro(){
    printf("Добро пожаловать в Akinator\n");
    printf("Что вы желаете:\n");
    printf("[О]тгадывать\n");
    printf("[D]ать определение\n");
    printf("[С]равнить объекты\n");
    printf("[P]окажи базу данных!\n");
}

static int getMode(){
    char mode[2] = {};
    fgets(mode, 2, stdin);

    while (true){
        if (strcmp(mode, "O") == 0 || strcmp(mode, "o") == 0){
            clearBuffer();
            return GUESS;
        }
        if (strcmp(mode, "D") == 0 || strcmp(mode, "d") == 0){
            clearBuffer();
            return DEFINITION;
        }
        if (strcmp(mode, "C") == 0 || strcmp(mode, "c") == 0){
            clearBuffer();
            return DIFFERENCE;
        }
        if (strcmp(mode, "P") == 0 || strcmp(mode, "p") == 0){
            clearBuffer();
            return DUMP;
        }

        clearBuffer();
        printf("Такого режима нет, попробуйте еще раз:\n");
        fgets(mode, 2, stdin);
    }
}

static int processMode(const int mode, BinDatabase* database){
    assert(database);

    switch(mode){
        case(GUESS):{
            playGuess(database, database->root);
            return NO_ERROR;
        }
        case(DEFINITION):{
            tellDefinition(database);
            return NO_ERROR;
        }
        case(DIFFERENCE):{
            tellDifference(database);
            return NO_ERROR;
        }
        case(DUMP):{
            binDatabaseDump(DUMP_DB(database), 0);
            return NO_ERROR;
        }
        default:{
            return UNKNOWN_MODE;
        }
    }
}
