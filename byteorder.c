#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/utsname.h>

int main (int argc, char **argv)
{
  struct utsname buffer;
  uname(&buffer);
  char sysInfo[sizeof(buffer) + 13];

  union {
    uint16_t s;
    char c[2];
  } un;

  un.s = 0x0102;

  if (un.c[0] == 1 ) {
    snprintf(sysInfo, sizeof(buffer) + 10, "%s %s: Big endian", buffer.machine, buffer.sysname, buffer.release);
  } else if (un.c[0] == 2) {
    snprintf(sysInfo, sizeof(buffer) + 13, "%s %s %s: Little endian", buffer.machine, buffer.sysname, buffer.release);
  }

  printf("%s\n", sysInfo);

  return 0;
}