#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_CLIENTS 2

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    exit(1);
  }

  int port = atoi(argv[1]);
  int sockfd;
  int n;
  socklen_t clilen;
  char line[1000];
  struct sockaddr_in servaddr, cliaddr[MAX_CLIENTS];

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
    perror(NULL);
    exit(1);
  }

  if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror(NULL);
    close(sockfd);
    exit(1);
  }

  int clients_count = 0;

  while (clients_count < MAX_CLIENTS) {
    clilen = sizeof(cliaddr[clients_count]);
    if ((n = recvfrom(sockfd, line, 999, 0,
                      (struct sockaddr *)&cliaddr[clients_count], &clilen)) <
        0) {
      perror(NULL);
      close(sockfd);
      exit(1);
    }
    clients_count++;
  }

  while (1) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
      clilen = sizeof(cliaddr[i]);
      if ((n = recvfrom(sockfd, line, 999, 0, (struct sockaddr *)&cliaddr[i],
                        &clilen)) < 0) {
        perror(NULL);
        close(sockfd);
        exit(1);
      }

      for (int j = 0; j < MAX_CLIENTS; j++) {
        if (j != i && cliaddr[j].sin_port != 0) {
          if (sendto(sockfd, line, strlen(line), 0,
                     (struct sockaddr *)&cliaddr[j], clilen) < 0) {
            perror(NULL);
            close(sockfd);
            exit(1);
          }
        }
      }
    }
  }

  return 0;
}
