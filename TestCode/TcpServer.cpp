#include <unistd.h>

#include <Networking/Sockets/SocketIPv4.hpp>
#include <future>

using namespace Networking::Sockets;

int connfd;

bool doWrite() {
  for (std::string line; std::getline(std::cin, line);) {
    write(connfd, line.c_str(), line.size());
  }
  return false;
}

bool doRead() {
  char buffer[4096];
  uint32_t bytesRead = 0;
  while ((bytesRead = read(connfd, buffer, 4097)) > 0) {
    std::cout << "Received data:\n" << std::string(buffer) << "\n";
    bzero(buffer, 4096);
  }
  return false;
}

int main() {
  int sockfd;
  int opt = 1;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt failed: ");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in aSockaddr;
  bzero(&aSockaddr, sizeof(aSockaddr));
  aSockaddr.sin_family = AF_INET;
  aSockaddr.sin_port = htons(7000);
  aSockaddr.sin_addr.s_addr = INADDR_ANY;
  bind(sockfd, (sockaddr *)&aSockaddr, sizeof(aSockaddr));

  listen(sockfd, 1024);

  socklen_t aSocklen = sizeof(aSockaddr);

  connfd = accept(sockfd, (sockaddr *)&aSockaddr, (socklen_t *)&aSocklen);

  auto aWriteAsync = std::async(std::launch::async, doWrite);
  auto aReadAsync = std::async(std::launch::async, doRead);

  aWriteAsync.get();
  aReadAsync.get();

  close(sockfd);
  close(connfd);

  return 1;
}