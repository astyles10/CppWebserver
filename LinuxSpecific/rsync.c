#include <stdio.h>

int main(int argc, char* argv[]) {
  printf("Called bad rsync with params:\n");
  int numArgs = argc;
  for (int i = 1; i < numArgs; i++) {
    printf("%s\n", argv[i]);
  }
  return 0;
}