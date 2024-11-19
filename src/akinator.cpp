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

//! TODO: читалка, запись в бд, 1) отгадывание и доб 2) определение объекта 3) разница между объектами 4) показать бд

static int databaseCtor(const char* db_filename, BinDatabase** database_tree INIT_ARGS);
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
static void clearBuffer();

int playAkinator(const char* db_filename){
    setlocale(LC_ALL,"Rus");

    BinDatabase* db = NULL;
    databaseCtor(db_filename, &db INIT(db));

    printIntro();
    int mode = getMode();
    printf("mode - %d\n", mode);

    return NO_ERROR;
}

static void printIntro(){
    printf("Добро пожаловать в Akinator\n");
    printf("Что вы желаете:\n");
    printf("[О]тгадывать\n");
    printf("[Д]ать определение\n");
    printf("[С]равнить объекты\n");
}

static int getMode(){
    char mode[2] = {};
    fgets(mode, 2, stdin);

    while (true){
        printf("MODE[0]: %d\n", mode[0]);
        printf("MODE[1]: %d\n", mode[1]);

        if (strcmp(mode, "О") == 0 || strcmp(mode, "о") == 0) return GUESS;
        if (strcmp(mode, "Д") == 0 || strcmp(mode, "д") == 0) return DEFINITION;
        if (strcmp(mode, "С") == 0 || strcmp(mode, "с") == 0) return DIFFERENCE;

        clearBuffer();
        printf("Такого режима нет, попробуйте еще раз:\n");
        fgets(mode, 2, stdin);
    }
}

static void clearBuffer(){
    int character = 0;
    while ((character = getchar()) != '\n' && character != EOF){}
}
//!  |
//! \|/
//!         read database from file

static int databaseCtor(const char* db_filename, BinDatabase** database_tree INIT_ARGS){
    assert(db_filename);
    assert(database_tree);

    char* database_txt = NULL;
    (*database_tree) = (BinDatabase*)calloc(1, sizeof(BinDatabase));

    INIT_DEBUG_VARS(database_tree);

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
