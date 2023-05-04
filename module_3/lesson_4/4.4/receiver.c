#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFSIZE 1024

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Usage: %s <port> <output_filename>\n", argv[0]);
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
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));

  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("bind");
    close(sockfd);
    return 1;
  }

  if (listen(sockfd, 1) < 0) {
    perror("listen");
    close(sockfd);
    return 1;
  }

  printf("Ожидаю входящее подключение...\n");

  struct sockaddr_in cli_addr;
  socklen_t cli_addrlen = sizeof(cli_addr);
  int connfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_addrlen);
  if (connfd < 0) {
    perror("accept");
    close(sockfd);
    return 1;
  }

  printf("Начинаю получать файл\n");

  int fd;
  if ((fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
    perror("open");
    close(connfd);
    close(sockfd);
    return 1;
  }

  int num_read;
  char fs[8];
  if ((num_read = recv(connfd, fs, 8, 0)) != 8) {
    fprintf(stderr, "Не получилось получить размер файла: получено %d байт\n",
            num_read);
    exit(1);
  }

  long filesize = *((long *)fs);
  long received_filesize = 0;

  char buf[BUFSIZE];
  while ((num_read = recv(connfd, buf, BUFSIZE, 0)) > 0) {
    received_filesize = received_filesize + num_read;
    printf("Передача файла: %.2f%% (%ld из %ld байт)\r",
           (float)received_filesize / filesize * 100.f, received_filesize,
           filesize);

    if (write(fd, buf, num_read) < 0) {
      perror("write");
      close(fd);
      close(connfd);
      close(sockfd);
      return 1;
    }
  }

  printf("\nФайл получен!\n");

  close(fd);
  close(connfd);
  close(sockfd);

  return 0;
}
