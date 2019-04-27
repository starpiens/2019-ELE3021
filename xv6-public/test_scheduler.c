/**
 * This program runs various workloads cuncurrently.
 */


#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
  int i;
  for (i = 15; i < 25; i++)
    printf(1, "%d ", cpu_share(i));

  exit();
}