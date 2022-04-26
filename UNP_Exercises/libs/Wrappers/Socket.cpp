#include "Socket.hpp"
#include <sys/socket.h>
#include <stdio.h>
#include <iostream>
#include <arpa/inet.h>
#include <strings.h>

/*
TODOs list
Socket library should contain all socket/networking headers, so users don't need
to include the wide array of headers to use sockets

1. Write functions to handle creating specific sockets - IPv4, IPv6, etc.
Use class inheritance since underlying functions are the same
2. Create separate classes for UDP & TCP
 */

namespace Wrappers
{
  Socket::Socket(const int newSockFd, const uint16_t family) : _sockfd(newSockFd), _family(family)
  {
  }

  Socket::~Socket()
  {
  }

  Socket* Socket::CreateSocket(const uint16_t family, const int type, const int protocol)
  {
    int newSockfd = 0;
    if ((newSockfd = socket(family, type, protocol)) < 0)
    {
      // TODO: perror uses errno, which does not get set when multi threading (threads return errno)
      perror("Socket::CreateSocket");
      return nullptr;
    }
    return new Socket(newSockfd, family);
  }

  Socket* Socket::CreateSocketIPv4()
  {
    return CreateSocket(AF_INET, SOCK_STREAM, 0);
  }

  Socket* Socket::CreateSocketIPv6()
  {
    return CreateSocket(AF_INET6, SOCK_STREAM, 0);
  }

  /* Server Functions */

  bool Socket::Listen(const int numListeners)
  {
    // TODO: ensure this isn't higher than the users allowed max files
    // See ulimit -n, or cat /proc/sys/fs/file-max
    if (listen(_sockfd, numListeners))
    {
      perror("listen");
      return false;
    }
    return true;
  }

  bool Socket::Bind(const uint32_t inAddr, const uint16_t port)
  {
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = _family;
    servaddr.sin_addr.s_addr = htonl(inAddr);
    servaddr.sin_port = htons(port);

    if (bind(_sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
      perror("bind");
      return false;
    }

    return true;
  }

  bool Socket::Bind(const std::string& presentationAddress, const uint16_t port)
  {
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = _family;
    servaddr.sin_addr.s_addr = htonl(inAddr);
    servaddr.sin_port = htons(port);

    if (bind(_sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
      perror("bind");
      return false;
    }

    return true;
  }

  /* Client Functions */

  bool Socket::Connect(const char *presentationAddress, const uint16_t port)
  {
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = _family;
    servaddr.sin_port = htons(port);

    // inet_pton returns -1 for invalid address family, 0 for invalid address and 1 for success
    // Sets errno on error
    if (inet_pton(_family, presentationAddress, &servaddr.sin_addr) < 0)
    {
      perror("inet_pton");
      return false;
    }

    // Socket functions from <sys/socket.h> that require pointers to sockaddr* structs
    // do so because socket functions predate ANSI C standard, so void * pointer type
    // was not available at the time of writing
    // connect returns -1 on fail and sets errno, else 0 for success
    if (connect(_sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
      perror("connect");
      return false;
    }

    return true;
  }

  /* Callbacks */

  void Socket::SetCallback(std::function<int(int, int)> callback)
  {
    fCallback = callback;
  }

  void Socket::RunCallback()
  {
    fCallback(1, 2);
  }

  const int Socket::GetSockFD(void) const
  {
    return _sockfd;
  }
}