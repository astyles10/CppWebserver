#include "Socket.hpp"
#include <sstream>

namespace Wrappers
{
  class SocketIPv4 : public Socket
  {
  public:
    typedef struct YStreamBuffer {
      size_t fBytesRead;
      std::stringstream fBufferData;
    } YStreamBuffer;

    static SocketIPv4 CreateTcpSocket();
    ~SocketIPv4();
    bool Bind(const uint32_t address, const uint16_t port);
    bool Bind(const std::string &pAddress, const uint16_t port);

    void OnAccept(std::function<std::string (struct sockaddr_in)> callback);
    void OnDataReceive(std::function<bool (std::string)> dataHandler);
    void Run();
    YStreamBuffer HandleRead(const int connfd);

  protected:
  private:
    SocketIPv4(const int type);
    void GetConnectionDetails(const struct sockaddr_in);

    std::function<std::string (struct sockaddr_in)> _acceptCallback;
    std::function<bool (std::string)> _dataHandlerCb;
  };

} // namespace Wrappers
