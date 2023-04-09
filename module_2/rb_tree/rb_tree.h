#pragma once

#define RB_PRINT_START 0

typedef enum { RED, BLACK } Color;

typedef struct RBNode {
    int value;
    Color color;
    struct RBNode* parent;
    struct RBNode* left;
    struct RBNode* right;
} RBNode;

RBNode* create_node(int value, Color color);

void insert(RBNode** tree, int value);

void print_tree(RBNode* tree, int depth);

void print_tree_graphviz(RBNode *node, FILE* file);
