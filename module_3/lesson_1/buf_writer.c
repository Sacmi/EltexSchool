#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 80

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *filename = argv[1];
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE];
    printf("Enter a line: ");
    fgets(line, MAX_LINE, stdin);
    fputs(line, file);

    fclose(file);

    return 0;
}