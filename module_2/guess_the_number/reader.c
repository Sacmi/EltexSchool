#include "reader.h"
#include <stdio.h>
#include <string.h>

#define INT_BUFFER_SIZE 10
#define DOUBLE_BUFFER_SIZE 20

void clearStdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int readInt(const char *prompt) {
    char buf[INT_BUFFER_SIZE + 1];
    int num;

    printf("%s\n", prompt);

    while (1) {
        while (fgets(buf, INT_BUFFER_SIZE + 1, stdin) == NULL) {
            printf("Ошибка: Некорректный ввод. Попробуйте еще раз:\n");
        }

        if (buf[strlen(buf) - 1] != '\n') {
            printf("Ошибка: Блин, много символов. Попробуйте еще раз:\n");
            clearStdin();
            continue;
        }

        if (sscanf(buf, "%d", &num) == 1) {
            return num;
        } else {
            printf("Ошибка: Введено не число. Попробуйте еще раз:\n");
        }
    }
}

double readDouble(const char *prompt) {
    char buf[DOUBLE_BUFFER_SIZE + 1];
    double num;

    printf("%s\n", prompt);

    while (1) {
        while (fgets(buf, DOUBLE_BUFFER_SIZE + 1, stdin) == NULL) {
            printf("Error: Некорректный ввод. Попробуйте еще раз:\n");
        }

        if (buf[strlen(buf) - 1] != '\n') {
            printf("Error: Блин, много символов. Попробуйте еще раз:\n");
            clearStdin();
            continue;
        }

        if (sscanf(buf, "%lf", &num) == 1) {
            return num;
        } else {
            printf("Error: Введено не число. Попробуйте еще раз:\n");
        }
    }
}
