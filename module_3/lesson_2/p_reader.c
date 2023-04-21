#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>

#define QUEUE "/ya_pidoras"
#define MSG_SIZE 255

int main(void) {
    mqd_t ds;
    struct mq_attr attr;

    if ((ds = mq_open(QUEUE, O_CREAT | O_RDONLY, 0600, NULL)) == -1) {
        perror("mq_open");
        exit(1);
    }

    if (mq_getattr(ds, &attr) == -1) {
        perror("mq_getattr");
        exit(1);
    }

    char *message = calloc(attr.mq_msgsize, sizeof(char));
    while (1) {
        unsigned int prio;
        if (mq_receive(ds, message, attr.mq_msgsize, &prio) == -1) {
            perror("mq_receive");
            exit(1);
        }

        if (prio == 255) {
            printf("Получен сигнал выхода\n");
            break;
        }

        printf("%d - %s\n", prio, message);
    }

    if (mq_close(ds) == -1) {
        perror("mq_close");
        exit(1);
    }

    if (mq_unlink(QUEUE) == -1) {
        perror("mq_unlink");
        exit(1);
    }

    return 0;
}