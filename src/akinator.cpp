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
    if (processMode(getMode(), db) == UNKNOWN_MODE) return UNKNOWN_MODE;

    databaseDtor(&db);

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
    char mode = 0;

    scanf(" %c", &mode);

    while (true){
        if (mode == 'o' || mode == 'O'){
            clearBuffer();
            return GUESS;
        }
        if (mode == 'd' || mode == 'D'){
            clearBuffer();
            return DEFINITION;
        }
        if (mode == 'c' || mode == 'C'){
            clearBuffer();
            return DIFFERENCE;
        }
        if (mode == 'p' || mode == 'P'){
            clearBuffer();
            return DUMP;
        }

        clearBuffer();
        printf("Такого режима нет, попробуйте еще раз:\n");
        scanf(" %c", &mode);
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
