#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char *argv[])
{
  if (argc != 2)
    return 1;
  char test[150] = {"TEST_VALUE="};
  strcat(test, argv[1]);
  putenv(test);
  const char * command = "/usr/bin/sshpass -p \"${TEST_VALUE}\" /usr/bin/rsync -ave /usr/bin/ssh ./testfile.txt alex@localhost:/home/alex/TestScripts";
  system(command);

  return 0;
}