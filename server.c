#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>

// https://www.csd.uoc.gr/~hy556/material/tutorials/cs556-3rd-tutorial.pdf
// https://www.tutorialspoint.com/http/http_responses.htm
// https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/

const short port = 3456;

int main (int argc, char* argv[]) {
  int sockfd, newSocket, valread;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};
  const char *hello = "Hello from server";

  if (!(sockfd = socket(AF_INET, SOCK_STREAM, 0))) {
    printf("Failed to open socket: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    perror("setsockopt failed: ");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  if (bind(sockfd, (struct sockaddr *)&address, addrlen) < 0) {
    perror("bind failed: ");
    exit(EXIT_FAILURE);
  }

  if (listen(sockfd, 3) < 0) {
    perror("Listen failed: ");
    exit(EXIT_FAILURE);
  }

  if ((newSocket = accept(sockfd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
    perror("Accept failed: ");
    exit(EXIT_FAILURE);
  }

  valread = read(newSocket, buffer, 1024);
  printf("%s\n", buffer);
  send(newSocket, hello, strlen(hello), 0);
  printf("Hello message sent\n");

  return 0;
}
