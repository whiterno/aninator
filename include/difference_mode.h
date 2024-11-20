#ifndef DIFFERENCE_MODE_H
#define DIFFERENCE_MODE_H

#include "../lib/stack/stack_hcpp/stack.h"

int printDifferences(BinDatabase* database, Stack* path1, char* object1, Stack* path2, char* object2);
int tellDifference(BinDatabase* database);

#endif
