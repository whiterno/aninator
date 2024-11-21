#ifndef READ_DATABASE_H
#define READ_DATABASE_H

int databaseCtor(const char* db_filename, BinDatabase** database_tree INIT_ARGS_DB);
int readFeature(BinDatabase** database_tree, char* database_txt, char** feature);

#endif
