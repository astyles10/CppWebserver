#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>

int main (int argc, char **argv)
{
  if (argc != 2)
  {
    printf("Usage: %s <ip-address>\n", argv[0]);
    return 1;
  }

  const char *address = argv[1];
  struct in_addr inaddr;
  struct in6_addr in6addr;
  char buf[INET_ADDRSTRLEN], buf6[INET_ADDRSTRLEN];

  if (inet_pton(AF_INET, address, &inaddr) == 1)
  {
    printf("IPv4 address parsed using inet_pton\n");
  }
  else if (inet_pton(AF_INET6, address, &in6addr) == 1)
  {
    printf("IPv6 address parsed using inet_pton\n");
  }
  else if (inet_aton(address, &inaddr))
  {
    printf("Used inet_aton\n");
  }
  else
  {
    printf("Couldn't even use inet_aton, exiting...\n");
    return 1;
  }

  const char* ipv4addr = inet_ntop(AF_INET, &inaddr, buf, INET_ADDRSTRLEN);
  const char* ipv6addr = inet_ntop(AF_INET6, &in6addr, buf6, INET6_ADDRSTRLEN);

  printf("IPv4 Address: %s\n", ipv4addr);
  printf("IPv6 Address: %s\n", ipv6addr);

  return 0;
}