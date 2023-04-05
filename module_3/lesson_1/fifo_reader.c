#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FIFO_PATH "/tmp/my_fifo"

int main() {
    int fd_fifo, buf;

    if ((fd_fifo = open(FIFO_PATH, O_RDONLY)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while (read(fd_fifo, &buf, sizeof(buf)) > 0) {
        printf("Прочитано число: %d\n", buf);
    }

    close(fd_fifo);

    return 0;
}