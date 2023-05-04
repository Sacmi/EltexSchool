#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAXLINE 1000

int main(int argc, char **argv) {
  int sockfd;
  int n;
  char sendline[MAXLINE], recvline[MAXLINE];
  struct sockaddr_in servaddr, cliaddr;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s <IP address> <port>\n", argv[0]);
    exit(1);
  }

  int port = atoi(argv[2]);

  if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
    perror(NULL);
    exit(1);
  }

  bzero(&cliaddr, sizeof(cliaddr));
  cliaddr.sin_family = AF_INET;
  cliaddr.sin_port = htons(0);
  cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr)) < 0) {
    perror(NULL);
    close(sockfd);
    exit(1);
  }

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);
  if (inet_aton(argv[1], &servaddr.sin_addr) == 0) {
    printf("Invalid IP address\n");
    close(sockfd);
    exit(1);
  }

  printf("Напишите `exit`, чтобы выйти из чата\n");

  while (1) {
    printf("Вы => ");
    fgets(sendline, MAXLINE, stdin);

    if (strncmp(sendline, "exit", 4) == 0) {
      printf("Выход из чата\n");
      break;
    }

    if (sendto(sockfd, sendline, strlen(sendline) + 1, 0,
               (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
      perror(NULL);
      close(sockfd);
      exit(1);
    }

    if ((n = recvfrom(sockfd, recvline, MAXLINE, 0, (struct sockaddr *)NULL,
                      NULL)) < 0) {
      perror(NULL);
      close(sockfd);
      exit(1);
    }

    printf("Ответ от сервера => %s", recvline);
  }

  close(sockfd);

  return 0;
}
