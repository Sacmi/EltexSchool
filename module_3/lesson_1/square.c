#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "USAGE: %s side1 side2 ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    int start = (pid == 0) ? 1 : (argc + 1) / 2;
    int end = (pid == 0) ? (argc + 1) / 2 : argc;

    for (int i = start; i < end; i++) {
        int side = atoi(argv[i]);
        int area = side * side;
        printf("Квадрат с стороной %d имеет площадь %d\n", side, area);
    }

    return 0;
}