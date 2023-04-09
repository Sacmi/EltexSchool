#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rb_tree.h"

#define FILENAME_SIZE 20

int main() {
    char *header = "digraph G {\n";
    char *footer = "}\n";

    RBNode *tree = NULL;

    for (int i = 5; i <= 30; i = i + 5) {
        printf("Inserting %d\n", i);
        insert(&tree, i);

        print_tree(tree, RB_PRINT_START);

        char filename[FILENAME_SIZE];
        sprintf(filename, "tree_%d.dot", i);

        FILE *file = fopen(filename, "w");
        if (file == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }

        fwrite(header, sizeof(char), strlen(header), file);
        print_tree_graphviz(tree, file);
        fwrite(footer, sizeof(char), strlen(footer), file);

        fclose(file);
        printf("\n");
    }

    return 0;
}
