#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "../include/akinator.h"

//! TODO: читалка, запись в бд, 1) отгадывание и доб 2) определение объекта 3) разница между объектами 4) показать бд
//! TODO: считывание названия файла с бд из консоли или main (подумать, что лучше)

static int databaseCtor(const char* db_filename, BinDatabase** database_tree);
static int readFile(const char* db_filename, char** database);
static int structDatabase(BinDatabase** database_tree, char* database_txt);
static size_t fileSize(const char* filename);
static int createNode(Node** node, char* feature);

int playAkinator(){
    BinDatabase* db = NULL;

    databaseCtor("database/main_db.db", &db);

    return NO_ERROR;
}

static int databaseCtor(const char* db_filename, BinDatabase** database_tree){
    assert(db_filename);
    assert(database_tree);

    char* database_txt = NULL;

    readFile(db_filename, &database_txt);
    structDatabase(database_tree, database_txt);

    return NO_ERROR;
}

static int structDatabase(BinDatabase** database_tree, char* database_txt){
    return NO_ERROR;
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
