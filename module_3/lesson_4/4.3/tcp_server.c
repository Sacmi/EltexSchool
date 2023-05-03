#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void dostuff(int);

void error(const char *msg)
{
  perror(msg);
  exit(1);
}

int nclients = 0;

void printusers()
{
  if (nclients)
  {
    printf("%d user on-line\n", nclients);
  }
  else
  {
    printf("No User on line\n");
  }
}

int myfunc(int a, int b) { return a + b; }

int main(int argc, char *argv[])
{
  printf("TCP SERVER DEMO\n");

  int sockfd, newsockfd;
  int portno;
  int pid;
  socklen_t clilen;
  struct sockaddr_in serv_addr, cli_addr;

  if (argc < 2)
  {
    fprintf(stderr, "ERROR, no port provided\n");
    exit(1);
  }
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");

  bzero((char *)&serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR on binding");
  listen(sockfd, 5);
  clilen = sizeof(cli_addr);

  while (1)
  {
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
      error("ERROR on accept");
    nclients++;
    printusers();

    pid = fork();
    if (pid < 0)
      error("ERROR on fork");
    if (pid == 0)
    {
      close(sockfd);
      dostuff(newsockfd);
      exit(0);
    }
    else
      close(newsockfd);
  }
  close(sockfd);
  return 0;
}

int sum(int a, int b) { return a + b; }

int subtract(int a, int b) { return a - b; }

int multiply(int a, int b) { return a * b; }

int divide(int a, int b) { return a / b; }

void dostuff(int sock)
{
  int bytes_recv;
  int a, b;
  char buff[1023];
#define str1 "Enter 1 parametr\r\n"
#define str2 "Enter 2 parametr\r\n"
#define str3 "Choose option (1 - sum, 2 - substract, 3 - multiply, 4 - divide)\r\n"

  write(sock, str1, sizeof(str1));

  bytes_recv = read(sock, &buff[0], sizeof(buff));
  if (bytes_recv < 0)
    error("ERROR reading from socket");
  a = atoi(buff);

  write(sock, str2, sizeof(str2));

  bytes_recv = read(sock, &buff[0], sizeof(buff));
  if (bytes_recv < 0)
    error("ERROR reading from socket");

  b = atoi(buff);

  write(sock, str3, sizeof(str3));
  bytes_recv = read(sock, &buff[0], sizeof(buff));
  if (bytes_recv < 0)
    error("ERROR reading from socket");

  int action = atoi(buff);
  switch (action)
  {
  case 1:
    a = sum(a, b);
    break;
  case 2:
    a = subtract(a, b);
    break;
  case 3:
    a = multiply(a, b);
    break;
  case 4:
    a = divide(a, b);
    break;
  default:
    a = -1;
    break;
  }

  snprintf(buff, strlen(buff), "%d\n", a);
  printf("%ld\n", strlen(buff));
  buff[strlen(buff)] = '\0';

  write(sock, &buff[0], sizeof(buff));

  nclients--;
  printf("-disconnect\n");
  printusers();
  return;
}
