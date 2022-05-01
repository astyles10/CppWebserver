#include "Socket.hpp"

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
  Socket::Socket()
  {
  }

  Socket::Socket(const uint16_t family, const int type, const int protocol)
  {
    if ((_sockfd = socket(family, type, protocol)) < 0)
    {
      // TODO: perror uses errno, which does not get set when multi threading (threads return errno)
      perror("Socket::CreateSocket");
    }
    _family = family;
    _type = type;
  }

  Socket::~Socket()
  {
  }

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
    return false;
  }

  bool Socket::Bind(const std::string &presentationAddress, const uint16_t port)
  {
    return false;
  }

  /* Client Functions */

  bool Socket::Connect(const char *pAddress, const uint16_t port)
  {
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = _family;
    servaddr.sin_port = htons(port);

    // inet_pton returns -1 for invalid address family, 0 for invalid address and 1 for success
    // Sets errno on error
    if (inet_pton(_family, pAddress, &servaddr.sin_addr) < 0)
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

  const bool Socket::Valid() const
  {
    return _sockfd >= 0;
  }

  const int Socket::GetSockFD(void) const
  {
    return _sockfd;
  }

  std::unique_ptr<Socket::YSockaddrGeneric> Socket::BuildSockaddr(const uint16_t port) const
  {
    std::unique_ptr<Socket::YSockaddrGeneric> aSockaddr = std::make_unique<YSockaddrGeneric>();
    bzero(aSockaddr.get(), sizeof(YSockaddrGeneric));
    aSockaddr->fIp4Sockaddr.sin_port = htons(port);
    aSockaddr->fIp4Sockaddr.sin_family = _family;
    return aSockaddr;
  }
}