#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../libs/Wrappers/Socket.hpp"
#include <limits.h>

using namespace Wrappers;

// TODO: Add this server as a systemd service

int testCb(int x, int y)
{
  printf("x + y = %d\n", (x + y));
  return 1;
}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    printf("Usage: %s <PORT>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int connfd;
  char *garbage = NULL; // Option to handle garbage characters
  const uint16_t port = strtoul(argv[1], &garbage, 0);

  Socket *aSocket = Socket::CreateSocket(AF_INET, SOCK_STREAM, 0);
  if (!aSocket)
  {
    printf("Could not create socket, exiting...\n");
    return 1;
  }

  aSocket->Bind(INADDR_ANY, port);
  aSocket->Listen(1024);

  char buff[4096];
  time_t ticks;

  // See https://blog.mbedded.ninja/programming/languages/c-plus-plus/callbacks/
  aSocket->SetCallback(&testCb);
  aSocket->RunCallback();

  aSocket->SetCallback([](int num1, int num2) -> int {
    return testCb(10, 24);
  });

  aSocket->RunCallback();
  struct sockaddr_in clientSocket;
  socklen_t sockLen;

  for (;;)
  {
    // TODO: Add callbacks for accept, on data, send data etc.
    if ((connfd = accept(aSocket->GetSockFD(), (struct sockaddr *)&clientSocket, &sockLen)) < 0)
    {
      perror("accept");
      return 1;
    }
    // char 
    printf("Connection details:\n");
    printf("Port: %d, ", clientSocket.sin_port);
    char clientAddress[16];
    const char* someValue = inet_ntop(AF_INET, &clientSocket.sin_addr, clientAddress, sizeof(clientSocket));
    printf("Client address: %s\n", clientAddress);
    printf("inet_pton return value: %s\n", someValue);

    ticks = time(NULL);
    snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
    for (int i = 0; i < strlen(buff); i++) {
      write(connfd, &buff[i], 1);
    }

    close(connfd);
  }
}