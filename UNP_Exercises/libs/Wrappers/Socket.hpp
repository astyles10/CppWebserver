#include <stdint.h>
#include <functional>
#include <string>
#include <sys/socket.h>
#include <stdio.h>
#include <iostream>
#include <arpa/inet.h>
#include <strings.h>
#include <memory>

namespace Wrappers
{
  class Socket
  {
  public:
    typedef union
    {
      struct sockaddr fSockaddr;
      struct sockaddr_in fIp4Sockaddr;
      struct sockaddr_in6 fIp6Sockaddr;
      struct sockaddr_storage fStorage;
    } YSockaddrGeneric;

    ~Socket();
    bool Listen(const int numListeners);
    bool Connect(const char *presentationAddress, const uint16_t port);
    void OnAccept(std::function<void()> callback);
    const int GetSockFD(void) const;
    const bool Valid() const;

  protected:
    Socket();
    Socket(const uint16_t family, const int type, const int protocol);

    virtual bool Bind(const uint32_t inAddr, const uint16_t port);
    virtual bool Bind(const std::string &presentationAddress, const uint16_t port);
    std::unique_ptr<YSockaddrGeneric> BuildSockaddr(const uint16_t port) const;

    int _sockfd;
    unsigned short _family;
    int _type;
    std::function<void()> fCallback;
  };
}
