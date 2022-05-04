#include <SocketIPv4.hpp>
#include <regex>

using namespace Wrappers;

class HttpParser
{
public:
  HttpParser();
  ~HttpParser();
  void OnData(const std::string &inData);
  const std::string& GetData(void) const;

private:
  std::string _data;
};

HttpParser::HttpParser(): _data({})
{
}

HttpParser::~HttpParser()
{
}

void HttpParser::OnData(const std::string &inData)
{
  std::regex aGetRequest("(?<=GET )(.*)(?= HTTP.*)");
  std::smatch aMatch;
  std::regex_search(inData, aMatch, aGetRequest);

  std::cout << "Matched data:\n";
  for (auto a : aMatch)
  {
    std::cout << "Page request = " << a << " ";
  }
  // std::regex aHttpVersion("(HTTP).*");
  std::cout << "\n";
}

const std::string &HttpParser::GetData(void) const
{
  return _data;
}

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

  HttpParser aHttpParser;

  auto aThingy = [&aHttpParser](std::string inData) -> bool {
    aHttpParser.OnData(inData);
    return false;
  };

  aSocket.OnDataReceive(aThingy);

  aSocket.Run();

  std::cout << "Http parser received: " << aHttpParser.GetData() << std::endl;

  return 0;
}