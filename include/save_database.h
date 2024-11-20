#ifndef SAVE_DATABASE_H
#define SAVE_DATABASE_H

int saveDatabase(const char* filename, BinDatabase* database);
int writeDatabase(FILE* fp, Node* node, int tabs);

#endif
