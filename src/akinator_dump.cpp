#include <stdio.h>
#include <stdlib.h>

#include "../include/akinator.h"
#include "../include/akinator_dump.h"

static int dump_img_counter = 0;
static int node_counter = 0;
static FILE* dump_html = fopen(DUMP_HTML_FILENAME, "w");

static const char* errorToString(int error);
static int createDotFile(Node* root);
static int createNodesAndEdges(FILE* dump_dot, Node* node, TreeDumpAttributes* attrs);


int binDatabaseDump(BinDatabase* tree, const char filename[], const char funcname[], const size_t line, int error){
    if (tree == NULL || error == NULL_VALUE_INSERTED){
        return NULL_VALUE_INSERTED;
    }
    node_counter = tree->nodes_amount;

    createDotFile(tree->root);

    fprintf(dump_html, "\n<pre>\n");
    fprintf(dump_html, "\tERROR: %s", errorToString(error));

    fprintf(dump_html, "\n\tDump called from %s:%s:%lu", filename, funcname, line);
    fprintf(dump_html, "\n\tTree %s born in %s:%s:%lu", tree->name, tree->filename, tree->funcname, tree->line);

    fprintf(dump_html, "\n\tBinary Database[%p]{", tree);
    fprintf(dump_html, "\n\t\troot = %p", tree->root);
    fprintf(dump_html, "\n\t\tnodes_amount = %d", tree->nodes_amount);
    fprintf(dump_html, "\n\t\t <img src = %d.png width = 50%%>", dump_img_counter - 1);
    fprintf(dump_html, "\n\n</pre>\n");

    return error;
}

static int createDotFile(Node* root){
    FILE* dump_dot = fopen(DUMP_DOT_FILENAME, "w");
    if (dump_dot == NULL){
        return FILE_NOT_OPEN;
    }

    TreeDumpAttributes attrs = {.node_color = "darkgreen",
                                .node_fillcolor = "white",
                                .node_shape = "record",
                                .edge_color = "black",
                                .edge_arrowhead = "normal"};

    fprintf(dump_dot, "digraph D{\n");
    fprintf(dump_dot, "\trankdir = TB\n\n");

    createNodesAndEdges(dump_dot, root, &attrs);

    fprintf(dump_dot, "}");
    fclose(dump_dot);

    char cmd[MAX_CMD_SIZE] = {};

    snprintf(cmd, MAX_CMD_SIZE, "dot ./log/dump.dot -Tpng -o ./log/%d.png", dump_img_counter++);
    system(cmd);

    return NO_ERROR;
}

static int createNodesAndEdges(FILE* dump_dot, Node* node, TreeDumpAttributes* attrs){
    if (node == NULL){
        return NO_ERROR;
    }
    if (node_counter < 0){
        return CYCLE_IN_TREE;
    }

    int node_serial_number = node_counter;
    fprintf(dump_dot, "\tnode_%d [color = \"%s\", style = \"filled\", shape = \"%s\", fillcolor = \"%s\", label = \"{%s | {<fl%d> left | <fr%d> right}}\"]\n", node_serial_number, attrs->node_color, attrs->node_shape, attrs->node_fillcolor, node->feature, node_serial_number, node_serial_number);
    if (node->left){
        fprintf(dump_dot, "\tnode_%d: <fl%d> -> node_%d [color = \"%s\", arrowhead = \"%s\"]\n", node_serial_number, node_serial_number, --node_counter, attrs->edge_color, attrs->edge_arrowhead);
        createNodesAndEdges(dump_dot, node->left, attrs);
    }
    if (node->right){
        fprintf(dump_dot, "\tnode_%d: <fr%d> -> node_%d [color = \"%s\", arrowhead = \"%s\"]\n", node_serial_number, node_serial_number, --node_counter, attrs->edge_color, attrs->edge_arrowhead);
        createNodesAndEdges(dump_dot, node->right, attrs);
    }

    return NO_ERROR;
}

#define _DESCR(error) case(error): return #error
static const char* errorToString(int error){
    switch (error)
    {
    _DESCR(NO_ERROR);
    _DESCR(NULL_VALUE_INSERTED);
    _DESCR(CYCLE_IN_TREE);
    _DESCR(FILE_NOT_OPEN);
    }

    return "IDK_YOU_IDIOT";
}
#undef _DESCR
