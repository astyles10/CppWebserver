#include <netinet/in.h>
#include <stdio.h>
#include <strings.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "../libs/Wrappers/Socket.hpp"
#include <iostream>
#include <unistd.h>

#include <iostream>

#define MAXLINE 4096

using namespace Wrappers;

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    printf("Usage: %s <IPAddress>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  Socket *aSocket = Socket::CreateSocket(AF_INET, SOCK_STREAM, 0);
  if (!aSocket)
  {
    std::cerr << "Could not create socket, exiting..." << std::endl;
    exit(EXIT_FAILURE);
  }

  aSocket->Connect(argv[1], 3457);

  int n;
  char recvline[MAXLINE + 1];

  // Always read socket data in a loop, no guarantee that TCP payload is provided in one packet
  // Unlike UDP, TCP has no 'record boundary', meaning a record (message) may be split into
  // multiple TCP packets.
  int readCount = 0;
  while ((n = read(aSocket->GetSockFD(), recvline, MAXLINE)) > 0)
  {
    readCount++;
    // Stream read is ended by closing the connection. This technique is used by HTTP
    // SMTP marks end of a record with two byte sequence: '\r' - '\n'
    // RPC and DNS place a binary count containing record length in front of each record
    // sent using TCP.
    recvline[n] = 0; // null terminate
    if (fputs(recvline, stdout) == EOF)
    {
      perror("fputs");
      exit(EXIT_FAILURE);
    }
    if (n < 0)
      printf("Read error\n");
  }

  // Unix always closes all open descriptors when process terminates.
  // i.e. sockfd is closed on exit
  printf("Read count = %d\n", readCount);
  exit(EXIT_SUCCESS);
}