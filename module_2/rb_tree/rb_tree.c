#include <stdio.h>
#include <stdlib.h>
#include "rb_tree.h"

RBNode* create_node(int value, Color color) {
    RBNode* node = malloc(sizeof(RBNode));
    node->value = value;
    node->color = color;
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void rotate_left(RBNode** tree, RBNode* node) {
    RBNode* right = node->right;
    node->right = right->left;
    if (right->left != NULL) {
        right->left->parent = node;
    }
    right->parent = node->parent;
    if (node->parent == NULL) {
        *tree = right;
    } else if (node == node->parent->left) {
        node->parent->left = right;
    } else {
        node->parent->right = right;
    }
    right->left = node;
    node->parent = right;
}

void rotate_right(RBNode** tree, RBNode* node) {
    RBNode* left = node->left;
    node->left = left->right;
    if (left->right != NULL) {
        left->right->parent = node;
    }
    left->parent = node->parent;
    if (node->parent == NULL) {
        *tree = left;
    } else if (node == node->parent->right) {
        node->parent->right = left;
    } else {
        node->parent->left = left;
    }
    left->right = node;
    node->parent = left;
}

void fix_insertion(RBNode** tree, RBNode* node) {
    while (node->parent != NULL && node->parent->color == RED) {
        RBNode* parent = node->parent;
        RBNode* grandparent = parent->parent;
        if (parent == grandparent->left) {
            RBNode* uncle = grandparent->right;
            if (uncle != NULL && uncle->color == RED) {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                node = grandparent;
            } else {
                if (node == parent->right) {
                    node = parent;
                    rotate_left(tree, node);
                }
                parent->color = BLACK;
                grandparent->color = RED;
                rotate_right(tree, grandparent);
            }
        } else {
            RBNode* uncle = grandparent->left;
            if (uncle != NULL && uncle->color == RED) {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                node = grandparent;
            } else {
                if (node == parent->left) {
                    node = parent;
                    rotate_right(tree, node);
                }
                parent->color = BLACK;
                grandparent->color = RED;
                rotate_left(tree, grandparent);
            }
        }
    }
    (*tree)->color = BLACK;
}

void insert(RBNode** tree, int value) {
    RBNode *node = create_node(value, RED);
    RBNode *parent = NULL;
    RBNode *current = *tree;
    while (current != NULL) {
        parent = current;
        if (value < current->value) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    node->parent = parent;
    if (parent == NULL) {
        *tree = node;
    } else if (value < parent->value) {
        parent->left = node;
    } else {
        parent->right = node;
    }
    fix_insertion(tree, node);
}

void print_tree(RBNode* node, int depth) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    printf("%d", node->value);
    if (node->color == RED) {
        printf(" (R)\n");
    } else {
        printf(" (B)\n");
    }

    print_tree(node->left, depth + 1);
    print_tree(node->right, depth + 1);
}

void print_tree_graphviz(RBNode *node, FILE* file) {
    if (node == NULL) {
        return;
    }

    // Создать узел с уникальным идентификатором, значением и цветом
    fprintf(file, "  %d [label=\"%d\", ", node->value, node->value);
    if (node->color == RED) {
        fprintf(file, "fillcolor=\"red\", style=\"filled\", fontcolor=\"white\"];\n");
    } else {
        fprintf(file, "fillcolor=\"black\", style=\"filled\", fontcolor=\"white\"];\n");
    }

    if (node->left != NULL) {
        // Создать связь между текущим узлом и левым поддеревом
        fprintf(file, "  %d -> %d;\n", node->value, node->left->value);
        print_tree_graphviz(node->left, file);
    }

    if (node->right != NULL) {
        // Создать связь между текущим узлом и правым поддеревом
        fprintf(file, "  %d -> %d;\n", node->value, node->right->value);
        print_tree_graphviz(node->right, file);
    }
}