#include <SocketIPv4.hpp>

using namespace Wrappers;

class HttpParser
{
public:
  HttpParser();
  ~HttpParser();
  void OnData(const std::string &inData);
};

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    printf("Usage: %s <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  const std::string &aPort = argv[1];

  char *garbage = NULL; // Option to handle garbage characters
  const uint16_t port = strtoul(aPort.c_str(), &garbage, 0);
  printf("Listening on port %d...\n", port);
  SocketIPv4 aSocket = SocketIPv4::CreateTcpSocket();
  aSocket.Bind(INADDR_ANY, port);
  aSocket.Listen(1024);

  aSocket.OnAccept([](struct sockaddr_in connection) -> std::string
                   {
  time_t ticks;
  char buff[4096];
  char clientAddress[16];
  inet_ntop(AF_INET, &connection.sin_addr, clientAddress, sizeof(connection));
  printf("Connection established from %s:%d\n", clientAddress, connection.sin_port);

  ticks = time(NULL);
  snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
  return std::string(buff); });

  aSocket.Run();

  return 0;
}