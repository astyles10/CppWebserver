#include "SocketIPv4.hpp"
#include <string.h>
#include <unistd.h>
#include <sstream>
#include <future>

#define BUFSIZE 4096

namespace Wrappers
{
  SocketIPv4::SocketIPv4(const int type) : Socket(AF_INET, type, 0)
  {
    int opt = 1;
    setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
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

  void SocketIPv4::OnDataReceive(std::function<std::string ()> dataHandler)
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

      auto SendMessages = [](const int connfd) {
        for (std::string line; std::getline(std::cin, line);) {
          line.push_back('\n');
          write(connfd, line.c_str(), line.size());
        }
      };

      const std::string aWrite = _acceptCallback(clientSocket);
      auto f = std::async(std::launch::async, &SocketIPv4::HandleRead, this, connfd);
      auto x = std::async(std::launch::async, SendMessages, connfd);
      write(connfd, aWrite.c_str(), aWrite.size());

      auto val = f.get();
      std::cout << "Read " << val.fBytesRead << "\n";
      std::cout << val.fBufferData.str() << "\n";
      close(connfd);
    }
  }

  SocketIPv4::YStreamBuffer SocketIPv4::HandleRead(const int connfd)
  {
    char buffer[BUFSIZE + 1];
    int bytesRead;
    YStreamBuffer aStreambuffer;

    while ( (bytesRead = read(connfd, buffer, BUFSIZE + 1)) > 0)
    {
      aStreambuffer.fBytesRead += bytesRead;
      aStreambuffer.fBufferData << buffer;
      memset(buffer, 0, BUFSIZE + 1);
      std::cout << "HandleRead read " << bytesRead << " bytes\n";
      
      break;
    }

    return aStreambuffer;
  }


} // namespace Wrappers
