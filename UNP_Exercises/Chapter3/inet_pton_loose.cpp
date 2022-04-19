#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <strings.h>

int main (int argc, char **argv)
{
  // inet_aton & inet_addr accept 1 to 4 bytes separated by dots, or a leading 0x for hex numbers.
  // inet_pton requires exactly 4 numbers separated by 3 dots.
  // inet_pton IPv6 addresses are not allowed to have dot-delimited addresses
  
  // TODO: write function called inet_pton_loose which handles these scenarios
  if (argc != 2)
  {
    printf("Usage: %s <ip-address>\n", argv[0]);
    return 1;
  }

  const char *address = argv[1];
  struct in_addr inaddr;
  struct in6_addr in6addr;
  char buf[INET_ADDRSTRLEN], buf6[INET_ADDRSTRLEN];

  // sockaddr_storage storage;
  // bzero(&storage, sizeof(storage));

  if (inet_pton(AF_INET, address, &inaddr) == 1)
  {
    printf("IPv4 Address parsed\n");
    return 0;
  }
  else if (inet_pton(AF_INET6, address, &in6addr) == 1)
  {
    printf("Valid IPv6 address using inet_pton %ld\n", in6addr.__in6_u.__u6_addr32);
    return 0;
  }
}