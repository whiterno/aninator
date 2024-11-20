#ifndef UTILS_H
#define UTILS_H

int readFile(const char* db_filename, char** database_txt);
size_t fileSize(const char* filename);
void fprintfnTab(FILE* fp, int n);
void clearBuffer();

#endif
