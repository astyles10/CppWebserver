#include "../libs/Wrappers/SocketIPv4.hpp"

using namespace Wrappers;

// TODO: Add this server as a systemd service
std::string ServerLoop(struct sockaddr_in clientSocket);

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    printf("Usage: %s <PORT>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char *garbage = NULL; // Option to handle garbage characters
  const uint16_t port = strtoul(argv[1], &garbage, 0);

  SocketIPv4 aSocket = SocketIPv4::CreateTcpSocket();
  if (!aSocket.Valid())
  {
    printf("Could not create socket, exiting...\n");
    return 1;
  }

  aSocket.Bind(INADDR_ANY, port);
  aSocket.Listen(1024);

  // See https://blog.mbedded.ninja/programming/languages/c-plus-plus/callbacks/

  aSocket.OnAccept(ServerLoop);
  aSocket.Run();
  return 0;
}

std::string ServerLoop(struct sockaddr_in connection)
{
  time_t ticks;
  char buff[4096];
  char clientAddress[16];
  inet_ntop(AF_INET, &connection.sin_addr, clientAddress, sizeof(connection));
  printf("Connection established from %s:%d\n", clientAddress, connection.sin_port);

  ticks = time(NULL);
  snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
  return std::string(buff);
}
