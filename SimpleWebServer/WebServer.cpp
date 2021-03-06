#include <Networking/Sockets/SocketIPv4.hpp>

#include "HttpParser.hpp"

using namespace Networking::Sockets;

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  const std::string &aPort = argv[1];

  char *garbage = NULL;  // Option to handle garbage characters
  const uint16_t port = strtoul(aPort.c_str(), &garbage, 0);
  printf("Listening on port %d...\n", port);
  SocketIPv4 aSocket = SocketIPv4::CreateTcpSocket();
  aSocket.Bind(INADDR_ANY, port);
  aSocket.Listen(1024);

  aSocket.OnAccept([&](struct sockaddr_in &connection) {
    char clientAddress[16];
    inet_ntop(AF_INET, &connection.sin_addr, clientAddress, sizeof(connection));
    printf("Connection established from %s:%d\n", clientAddress,
           connection.sin_port);
  });

  HttpParser aHttpParser;

  auto aThingy = [&aHttpParser](const std::string &inData) -> std::string {
    const std::string &aWriteBackValue = aHttpParser.OnData(inData);
    return aWriteBackValue;
  };

  aHttpParser.SetPostRequestHandler(
      "test", [&](const std::string &inData) -> HttpParser::HttpResponse {
        std::cout << "Received post request to /test\n";
        HttpParser::HttpResponse aResponse;
        aResponse.fDescription = "OK";
        aResponse.fResponseCode = 200;
        aResponse.fConnection = "close";
        aResponse.fContentType = "text/plain";
        aResponse.fData = "Received POST!";

        return aResponse;
      });

  aSocket.OnDataReceive(aThingy, "\r\n\r\n");
  aSocket.Run();
  return 0;
}