#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "../include/akinator.h"
#include "../include/utils.h"

int readFile(const char* db_filename, char** database_txt){
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

size_t fileSize(const char* filename){
    assert(filename);

    struct stat file_stat;
    stat(filename, &file_stat);
    return file_stat.st_size;
}

void fprintfnTab(FILE* fp, int n){
    for (int i = 0; i < n; i++){
        fputc('\t', fp);
    }
}

void clearBuffer(){
    int character = 0;
    while ((character = getchar()) != '\n' && character != EOF){}
}
