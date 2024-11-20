#ifndef READ_DATABASE_H
#define READ_DATABASE_H

int databaseCtor(const char* db_filename, BinDatabase** database_tree INIT_ARGS_DB);
int structRoot(BinDatabase** database_tree, char* database_txt);
int structDatabase(BinDatabase** database_tree, char** database_txt, Node* parent);
int readFeature(BinDatabase** database_tree, char* database_txt, char** feature);
int structLeftTree(BinDatabase** database_tree, char** database_txt, Node* parent);
int structRightTree(BinDatabase** database_tree, char** database_txt, Node* parent);

#endif
