#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <clocale>

#include "../include/akinator.h"
#include "../include/akinator_dump.h"
#include "../lib/stack/stack_hcpp/stack.h"

static int databaseCtor(const char* db_filename, BinDatabase** database_tree INIT_ARGS_DB);
static int structRoot(BinDatabase** database_tree, char* database_txt);
static int structDatabase(BinDatabase** database_tree, char** database_txt, Node* parent);
static int readFeature(BinDatabase** database_tree, char* database_txt, char** feature);
static int structRightTree(BinDatabase** database_tree, char** database_txt, Node* parent);
static int structLeftTree(BinDatabase** database_tree, char** database_txt, Node* parent);

static int bigArrayCtor(BinDatabase** database_tree);
static int addToBigArray(BinDatabase** database_tree, char* database_txt, char** feature, int feature_len);

static int createNode(Node** node, char* feature);
static int fillNode(BinDatabase** database_tree, char** database_txt, Node** node);
static int isNodeEmpty(char* begin);

static int readFile(const char* db_filename, char** database);
static size_t fileSize(const char* filename);

static int saveDatabase(const char* filename, BinDatabase* database);
static int writeDatabase(FILE* fp, Node* node, int tabs);
static void fprintfnTab(FILE* fp, int n);

static void printIntro();
static int getMode();
static int processMode(int mode, BinDatabase* database);
static void clearBuffer();

static int playGuess(BinDatabase* database, Node* node);
static int addNode(BinDatabase* database, Node* node);
static int getAns();

static int tellDefinition(BinDatabase* database);
static int findNode(Node* node, char* object, Node** required_node, Stack* path);
static int printObjectFeatures(Node* node, Stack* path, char* object);

static void reverseStack(Stack* stk);
static void fromStkToStk(Stack* stk1, Stack* stk2);

static int printDifferences(BinDatabase* database, Stack* path1, char* object1, Stack* path2, char* object2);
static int tellDifference(BinDatabase* database);

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

static int processMode(int mode, BinDatabase* database){
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

static void clearBuffer(){
    int character = 0;
    while ((character = getchar()) != '\n' && character != EOF){}
}

//!  |
//! \|/
//!         tell difference

static int tellDifference(BinDatabase* database){
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

    Node* req_node1 = NULL;
    Stack* path1 = stackCtor(INIT(path1) 0);
    findNode(database->root, object1, &req_node1, path1);
    if (req_node1 == NULL){
        printf("Нет такого объекта в базе данных!\n");
        return NO_OBJECT_IN_DB;
    }

    Node* req_node2 = NULL;
    Stack* path2 = stackCtor(INIT(path2) 0);
    findNode(database->root, object2, &req_node2, path2);
    if (req_node2 == NULL){
        printf("Нет такого объекта в базе данных!\n");
        return NO_OBJECT_IN_DB;
    }

    printDifferences(database, path1, object1, path2, object2);
    stackDtor(&path1);
    stackDtor(&path2);

    return NO_ERROR;
}

static int printDifferences(BinDatabase* database, Stack* path1, char* object1, Stack* path2, char* object2){
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
        if (stackPop(path1, &step1) == 4) return NO_ERROR;
        stackPop(path2, &step2);

        if (step1 != step2){
            break;
        }
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
    printObjectFeatures(ptr1, path1, object1);
    printObjectFeatures(ptr2, path2, object2);

    return NO_ERROR;
}

//! /|\
//!  |
//!         tell difference



//!  |
//! \|/
//!         tell definition

static int tellDefinition(BinDatabase* database){
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

static int printObjectFeatures(Node* node, Stack* path, char* object){
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

static void reverseStack(Stack* stk){
    Stack* tmp1 = stackCtor(INIT(tmp1) 0);
    Stack* tmp2 = stackCtor(INIT(tmp2) 0);

    fromStkToStk(stk, tmp1);
    fromStkToStk(tmp1, tmp2);
    fromStkToStk(tmp2, stk);

    stackDtor(&tmp1);
    stackDtor(&tmp2);
}

static void fromStkToStk(Stack* stk1, Stack* stk2){
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

static int findNode(Node* node, char* object, Node** required_node, Stack* path){
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



//! /|\
//!  |
//!         tell definition

//!  |
//! \|/
//!         play guess

static int playGuess(BinDatabase* database, Node* node){
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

static int addNode(BinDatabase* database, Node* node){
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


//! /|\
//!  |
//!         play guess



//!  |
//! \|/
//!         read database from file

static int databaseCtor(const char* db_filename, BinDatabase** database_tree INIT_ARGS_DB){
    assert(db_filename);
    assert(database_tree);

    char* database_txt = NULL;
    (*database_tree) = (BinDatabase*)calloc(1, sizeof(BinDatabase));

    INIT_DEBUG_VARS_DB(database_tree);

    readFile(db_filename, &database_txt);
    bigArrayCtor(database_tree);
    structRoot(database_tree, database_txt);

    return NO_ERROR;
}

static int structRoot(BinDatabase** database_tree, char* database_txt){
    assert(database_tree);
    assert(database_txt);

    database_txt = strchr(database_txt, '{');
    if (database_txt == NULL || isNodeEmpty(database_txt + 1)) return NO_DATA_BASE;

    database_txt = strchr(database_txt, '"');
    char* feature = NULL;
    readFeature(database_tree, database_txt, &feature);
    database_txt = strchr(database_txt + 1, '"') + 1;

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

    structLeftTree(database_tree, database_txt, parent);
    structRightTree(database_tree, database_txt, parent);

    return NO_ERROR;
}

static int readFeature(BinDatabase** database_tree, char* database_txt, char** feature){
    assert(database_tree);
    assert(database_txt);
    assert(feature);

    char* end = strchr(database_txt + 1, '"');
    int feature_len = end - database_txt;

    if (feature_len > BIG_ARR_ELEM_CAPACITY){
        return FAT_FEATURE;
    }

    addToBigArray(database_tree, database_txt, feature, feature_len);

    return NO_ERROR;
}

static int structLeftTree(BinDatabase** database_tree, char** database_txt, Node* parent){
    assert(database_tree);
    assert(database_txt);
    assert(parent);

    Node* node = NULL;
    fillNode(database_tree, database_txt, &node);
    parent->left = node;

    (*database_tree)->nodes_amount++;

    structDatabase(database_tree, database_txt, parent->left);

    return NO_ERROR;
}

static int structRightTree(BinDatabase** database_tree, char** database_txt, Node* parent){
    assert(database_tree);
    assert(database_txt);
    assert(parent);

    Node* node = NULL;
    fillNode(database_tree, database_txt, &node);
    parent->right = node;

    (*database_tree)->nodes_amount++;

    structDatabase(database_tree, database_txt, parent->right);

    return NO_ERROR;
}

static int bigArrayCtor(BinDatabase** database_tree){
    assert(database_tree);

    (*database_tree)->big_array_capacity = BIG_ARR_BEG_CAPACITY;
    (*database_tree)->big_array = (BigArrElem*)calloc(BIG_ARR_BEG_CAPACITY, sizeof(BigArrElem));
    for (int i = 0; i < BIG_ARR_BEG_CAPACITY; i++){
        (*database_tree)->big_array[i].beg = (char*)calloc(BIG_ARR_ELEM_CAPACITY, sizeof(char));
        (*database_tree)->big_array[i].end = (*database_tree)->big_array[i].beg;
    }

    return NO_ERROR;
}

static int addToBigArray(BinDatabase** database_tree, char* database_txt, char** feature, int feature_len){
    assert(database_tree);
    assert(database_txt);
    assert(feature);

    for (int i = 0; i < (*database_tree)->big_array_capacity; i++){
        int left_space = BIG_ARR_ELEM_CAPACITY - ((*database_tree)->big_array[i].end - (*database_tree)->big_array[i].beg + 1);

        if (feature_len > left_space){
            continue;
        }

        strncpy((*database_tree)->big_array[i].end + 1, database_txt + 1, feature_len - 1);
        (*feature) = (*database_tree)->big_array[i].end + 1;

        (*database_tree)->big_array[i].end = (*database_tree)->big_array[i].end + feature_len;
        *(*database_tree)->big_array[i].end = '\0';

        return NO_ERROR;
    }

    //! TODO: сделать расширяемый bigArray

    return NO_ERROR;
}

static int createNode(Node** node, char* feature){
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

static int fillNode(BinDatabase** database_tree, char** database_txt, Node** node){
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

static int isNodeEmpty(char* begin){
    char* end = strchr(begin, '}');

    while (begin != end){
        if (*begin != ' '){
            return NOT_EMPTY;
        }
        begin++;
    }

    return EMPTY;
}

static int readFile(const char* db_filename, char** database_txt){
    assert(db_filename);
    assert(database_txt);

    size_t file_size = fileSize(db_filename);
    int fp = open(db_filename, O_RDONLY);

    if (fp == -1){
        return NO_SUCH_FILE;
    }

    (*database_txt) = (char*)calloc(file_size + 1, sizeof(char));
    read(fp, *database_txt, file_size);
    close(fp);

    (*database_txt)[file_size] = '\0';

    return NO_ERROR;
}

static size_t fileSize(const char* filename){
    assert(filename);

    struct stat file_stat;
    stat(filename, &file_stat);
    return file_stat.st_size;
}

//! /|\
//!  |
//!         read database from file

//!  |
//! \|/
//!         write database to file

static int saveDatabase(const char* filename, BinDatabase* database){
    assert(filename);
    assert(database);

    FILE* fp = fopen(filename, "w");
    if (fp == NULL) return FILE_NOT_OPEN;

    writeDatabase(fp, database->root, 0);

    return NO_ERROR;
}

static int writeDatabase(FILE* fp, Node* node, int tabs){
    assert(fp);
    assert(node);

    fprintfnTab(fp, tabs);
    fprintf(fp, "{ \"%s\" ", node->feature);

    if (node->left != NULL){
        fprintf(fp, "\n");
        writeDatabase(fp, node->left, tabs + 1);
        writeDatabase(fp, node->right, tabs + 1);
    }
    if (node->left != NULL){
        fprintfnTab(fp, tabs);
        fprintf(fp, "}\n");
    }
    else{
        fprintf(fp, "}\n");
    }

    return NO_ERROR;
}

static void fprintfnTab(FILE* fp, int n){
    for (int i = 0; i < n; i++){
        fputc('\t', fp);
    }
}

//! /|\
//!  |
//!         write database to file
