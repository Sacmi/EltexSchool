#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define QUEUE "/ya_pidoras"
#define MSG_SIZE 256
#define MSG_PRIORITY 1
#define END_PRIORITY 255

int main(void) {
    mqd_t ds;
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 6;
    attr.mq_msgsize = MSG_SIZE;
    attr.mq_curmsgs = 0;

    if ((ds = mq_open(QUEUE, O_CREAT | O_WRONLY | O_NONBLOCK, 0666, &attr)) == -1) {
        perror("mq_open");
        exit(1);
    }

    char message[MSG_SIZE];
    srand(time(NULL));

    for (int i = 0; i < 5; ++i) {
        int random = rand();
        snprintf(message, MSG_SIZE, "%d", random);

        if (mq_send(ds, message, strlen(message), MSG_PRIORITY) == -1) {
            perror("mq_send");
            exit(1);
        }
    }

    sleep(1);

    if (mq_send(ds, "", 0, END_PRIORITY) == -1) {
        perror("mq_send");
        exit(1);
    }

    if (mq_close(ds) == -1) {
        perror("mq_close");
        exit(1);
    }

    return 0;
}