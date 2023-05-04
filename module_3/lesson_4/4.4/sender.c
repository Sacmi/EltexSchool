#include <arpa/inet.h>
#include <fcntl.h>
#include <math.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFSIZE 1024

int main(int argc, char *argv[]) {
  if (argc < 4) {
    fprintf(stderr, "Usage: %s <ip_address> <port> <filename>\n", argv[0]);
    return 1;
  }

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("socket");
    return 1;
  }

  struct sockaddr_in serv_addr;

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(atoi(argv[2]));

  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("connect");
    close(sockfd);
    return 1;
  }

  printf("Подключен к получателю!\n");

  struct stat st;
  if (stat(argv[3], &st) < 0) {
    perror("stat");
    exit(1);
  }

  char fs[8];
  *((long *)fs) = st.st_size;
  if (send(sockfd, fs, 8, 0) < 1) {
    perror("send");
    exit(1);
  }

  int fd;
  if ((fd = open(argv[3], O_RDONLY)) < 0) {
    perror("open");
    close(sockfd);
    return 1;
  }

  printf("Отправка файла\n");

  char buf[BUFSIZE];
  int num_read;
  size_t counter = 0;
  size_t total_packets = ceilf((float)st.st_size / BUFSIZE);
  while ((num_read = read(fd, buf, BUFSIZE)) > 0) {
    if (send(sockfd, buf, num_read, 0) < 0) {
      perror("send");
      close(fd);
      close(sockfd);
      return 1;
    }

    counter++;
    printf("Отправлено пакетов: %ld из %ld\r", counter, total_packets);
  }

  printf("\nФайл отправлен!\n");

  close(fd);
  close(sockfd);

  return 0;
}
