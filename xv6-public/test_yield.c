#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int pid = fork();
  while (1) {
    if (pid == 0) {
      printf(1, "Child\n");
      yield();
    } else {
      printf(1, "Parent\n");
      yield();
    }
  }
  return 0;
}
