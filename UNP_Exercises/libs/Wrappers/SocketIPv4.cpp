#include "SocketIPv4.hpp"
#include <string.h>
#include <unistd.h>

namespace Wrappers
{
  SocketIPv4::SocketIPv4(const int type) : Socket(AF_INET, type, 0)
  {
  }

  SocketIPv4::~SocketIPv4()
  {
  }

  SocketIPv4 SocketIPv4::CreateTcpSocket()
  {
    return SocketIPv4(SOCK_STREAM);
  }

  bool SocketIPv4::Bind(const uint32_t address, const uint16_t port)
  {
    std::unique_ptr<YSockaddrGeneric> aSockaddr = BuildSockaddr(port);
    aSockaddr->fIp4Sockaddr.sin_addr.s_addr = htonl(address);

    if (bind(_sockfd, (struct sockaddr *)&aSockaddr->fIp4Sockaddr, sizeof(aSockaddr->fIp4Sockaddr)) < 0)
    {
      perror("bind");
      return false;
    }
    return true;
  }

  bool SocketIPv4::Bind(const std::string &pAddress, const uint16_t port)
  {
    struct in_addr aAddress;
    if (inet_pton(_family, pAddress.c_str(), &aAddress) < 0)
    {
      perror("inet_pton");
      return false;
    }
    return Bind(aAddress.s_addr, port);
  }

  void SocketIPv4::OnAccept(std::function<std::string(struct sockaddr_in)> onAcceptCallback)
  {
    _acceptCallback = onAcceptCallback;
  }

  void SocketIPv4::OnDataReceive(std::function<bool(std::string)> dataHandler)
  {
    _dataHandlerCb = dataHandler;
  }

  void SocketIPv4::Run()
  {
    int connfd;
    struct sockaddr_in clientSocket;
    socklen_t sockLen;
    for (;;)
    {
      printf("Waiting...\n");
      if ((connfd = accept(_sockfd, (struct sockaddr *)&clientSocket, &sockLen)) < 0)
      {
        perror("accept");
        return;
      }

      const std::string aWrite = _acceptCallback(clientSocket);
      char recvline[4097];
      int n;
      while ((n = recv(connfd, recvline, 4097, 0)) > 0)
      {
        recvline[n] = 0;
        if (!_dataHandlerCb(std::string(recvline)))
        {
          break;
        }
      }
      bzero(recvline, sizeof(recvline));

      write(connfd, aWrite.c_str(), aWrite.size()); // Blocking
      close(connfd);
    }
  }

} // namespace Wrappers
