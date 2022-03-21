#include "Socket.hpp"
#include <sys/socket.h>
#include <stdio.h>
#include <iostream>
#include <arpa/inet.h>
#include <strings.h>

namespace Wrappers
{
  Socket *Socket::CreateSocket(const int family, const int type, const int protocol)
  {
    int newSockfd = 0;
    if ((newSockfd = socket(family, type, protocol)) < 0)
    {
      // TODO: perror uses errno, which does not get set when multi threading
      perror("Socket::CreateSocket");
      return nullptr;
    }
    // return new Socket(newSockfd);
    return new Socket(newSockfd);
  }

  Socket::Socket(const int newSockFd) : sockfd(newSockFd)
  {
  }

  Socket::~Socket()
  {
  }

  int Socket::Listen()
  {
    return 0;
  }

  int Socket::Bind(const uint8_t addressFamily, const uint32_t inAddr, const uint16_t port)
  {
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = addressFamily;
    servaddr.sin_addr.s_addr = htonl(inAddr);
    servaddr.sin_port = htons(port); /* daytime server */
  }

  int Socket::Connect(const int addressFamily, const char *address, const unsigned short port)
  {
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = addressFamily;
    servaddr.sin_port = htons(port);

    // inet_pton returns -1 for invalid address family, 0 for invalid address and 1 for success
    // Sets errno on error
    if (inet_pton(addressFamily, address, &servaddr.sin_addr) < 0)
    {
      perror("inet_pton");
      return 0;
    }

    // Socket functions from <sys/socket.h> that require pointers to sockaddr* structs
    // do so because socket functions predate ANSI C standard, so void * pointer type
    // was not available at the time of writing
    // connect returns -1 on fail and sets errno, else 0 for success
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
      perror("connect");
      return 0;
    }

    return 1;
  }

  int Socket::GetSockFD(void)
  {
    return sockfd;
  }

}