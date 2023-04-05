#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE 80

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *filename = argv[1];
    int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE];
    printf("Enter a line: ");
    fgets(line, MAX_LINE, stdin);

    if (write(fd, line, strlen(line)) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    close(fd);

    return 0;
}