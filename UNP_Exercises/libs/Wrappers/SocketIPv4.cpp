#include "SocketIPv4.hpp"

#include <string.h>
#include <unistd.h>

#include <future>
#include <sstream>

#define BUFSIZE 4096

namespace Wrappers {
SocketIPv4::SocketIPv4(const int type) : Socket(AF_INET, type, 0) {
  int opt = 1;
  setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
             sizeof(opt));
}

SocketIPv4::~SocketIPv4() {}

SocketIPv4 SocketIPv4::CreateTcpSocket() { return SocketIPv4(SOCK_STREAM); }

bool SocketIPv4::Bind(const uint32_t address, const uint16_t port) {
  std::unique_ptr<YSockaddrGeneric> aSockaddr = BuildSockaddr(port);
  aSockaddr->fIp4Sockaddr.sin_addr.s_addr = htonl(address);

  if (bind(_sockfd, (struct sockaddr *)&aSockaddr->fIp4Sockaddr,
           sizeof(aSockaddr->fIp4Sockaddr)) < 0) {
    perror("bind");
    return false;
  }
  return true;
}

bool SocketIPv4::Bind(const std::string &pAddress, const uint16_t port) {
  struct in_addr aAddress;
  if (inet_pton(_family, pAddress.c_str(), &aAddress) < 0) {
    perror("inet_pton");
    return false;
  }
  return Bind(aAddress.s_addr, port);
}

void SocketIPv4::OnAccept(
    std::function<void(struct sockaddr_in &)> onAcceptCallback) {
  _acceptCallback = onAcceptCallback;
}

void SocketIPv4::OnDataReceive(
    std::function<std::string(const std::string &inData)> dataHandler,
    const std::string &terminator = "") {
  _dataHandlerCb = dataHandler;
  _terminator = terminator;
}

void SocketIPv4::Run() {
  int connfd;
  struct sockaddr_in clientSocket;
  socklen_t sockLen;
  pid_t aPid;
  for (;;) {
    printf("Waiting...\n");
    if ((connfd = accept(_sockfd, (struct sockaddr *)&clientSocket, &sockLen)) <
        0) {
      perror("accept");
      return;
    }

    if ((aPid = fork()) == 0) {
      close(_sockfd);
      if (_acceptCallback) {
        _acceptCallback(clientSocket);
      }

      auto aAsyncRead =
          std::async(std::launch::async, &SocketIPv4::HandleRead, this, connfd);
      YStreamBuffer aReadBuffer = aAsyncRead.get();

      if (_dataHandlerCb) {
        const std::string &aWriteData =
            _dataHandlerCb(aReadBuffer.fBufferData.str());
        write(connfd, aWriteData.c_str(), aWriteData.size());
      }
      close(connfd);
      exit(0);
    }

    close(connfd);
  }
}

static inline bool endsWith(const std::string &str, const std::string &suffix) {
  return str.size() >= suffix.size() &&
         0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

SocketIPv4::YStreamBuffer SocketIPv4::HandleRead(const int connfd) {
  char buffer[BUFSIZE + 1];
  int bytesRead;
  YStreamBuffer aStreambuffer;

  std::cout << "Waiting for data...\n";
  while ((bytesRead = read(connfd, buffer, BUFSIZE + 1)) > 0) {
    buffer[bytesRead] = 0;
    aStreambuffer.fBytesRead += bytesRead;
    aStreambuffer.fBufferData << buffer;
    if (endsWith(aStreambuffer.fBufferData.str(), _terminator)) {
      std::cout << "HandleRead read " << bytesRead << " bytes\n";
      break;
    }

    memset(buffer, 0, BUFSIZE + 1);
  }

  return aStreambuffer;
}

}  // namespace Wrappers
