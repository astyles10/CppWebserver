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

int testCb(int x, int y)
{
  printf("x + y = %d\n", (x + y));
  return 1;
}

int main(int argc, char **argv)
{
  int connfd;
  if (argc != 2)
  {
    printf("Usage: %s <PORT>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char *garbage = NULL;
  const uint16_t port = strtoul(argv[1], &garbage, 0);

  Socket *aSocket = Socket::CreateSocket(AF_INET, SOCK_STREAM, 0);
  if (!aSocket)
  {
    printf("Could not create socket, exiting...\n");
    return 1;
  }

  aSocket->Bind(AF_INET, INADDR_ANY, port);
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

  for (;;)
  {
    // TODO: Add callbacks for accept, on data, send data etc.
    if ((connfd = accept(aSocket->GetSockFD(), (struct sockaddr *)NULL, NULL)) < 0)
    {
      perror("accept");
      return 1;
    }

    ticks = time(NULL);
    snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
    write(connfd, buff, strlen(buff));

    close(connfd);
  }
}