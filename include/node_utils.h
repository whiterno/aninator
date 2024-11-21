#ifndef NODE_UTILS_H
#define NODE_UTILS_H

#include "../lib/stack/stack_hcpp/stack.h"

int createNode(Node** node, char* feature);
int fillNode(BinDatabase** database_tree, char** database_txt, Node** node);
int isNodeEmpty(char* begin);
int findNode(Node* node, const char* object, Node** required_node, Stack* path);
int addNode(BinDatabase* database, Node* node);
int findPath(BinDatabase* database, const char object[], Stack* path);

#endif
