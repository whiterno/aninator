#ifndef DEFINITION_MODE_H
#define DEFINITION_MODE_H

#include "../lib/stack/stack_hcpp/stack.h"

int tellDefinition(BinDatabase* database);
int printObjectFeatures(Node* node, Stack* path, char* object);

#endif
