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

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    printf("Usage: %s <PORT>\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  char *garbage = NULL;
  const uint16_t port = strtoul(argv[2], &garbage, 0);

  Socket *aSocket = Socket::CreateSocket(AF_INET, SOCK_STREAM, 0);
  if (!aSocket)
  {
    printf("Could not create socket, exiting...\n");
    return 1;
  }

  aSocket->Bind(AF_INET, INADDR_ANY, port);
  aSocket->Listen();

  char buff[4096];
  time_t ticks;

  

  if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
  {
    perror("bind");
    return 1;
  }

  if (listen(listenfd, 1024) < 0)
  {
    perror("listen");
    return 1;
  }

  for (;;)
  {
    if ((connfd = accept(listenfd, (struct sockaddr *)NULL, NULL)) < 0)
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