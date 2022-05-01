#include <iostream>
#include "unpConfig.h"
#include "libs/Wrappers/SocketIPv4.hpp"
#include <arpa/inet.h>

using namespace Wrappers;

int main (int argc, char **argv) {
  std::cout << "Hello Cmake" << std::endl;
  if (argc < 2) {
    std::cout << argv[0] << " Version" << UNP_VERSION_MAJOR << "."
              << UNP_VERSION_MINOR << std::endl;
    std::cout << "Usage: " << argv[0] << " <IP Address>" << std::endl;
    return 1;
  }

  SocketIPv4 aSocket = SocketIPv4::CreateTcpSocket();

  if (aSocket.GetSockFD() > 0) {
    std::cout << "Socket file descriptor successful" << std::endl;
    std::cout << "File descriptor number = " << aSocket.GetSockFD() << std::endl;
  } else {
    std::cout << "Could not create socket..." << std::endl;
  }
  return 0;
}