#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  printf("%d\n", getpid());
  printf("%d\n", getppid());
  printf("%d\n", getuid());
  printf("%d\n", geteuid());
  printf("%d\n", getgid());

  return 0;
}
