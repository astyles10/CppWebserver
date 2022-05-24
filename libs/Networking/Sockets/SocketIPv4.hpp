#include <sstream>

#include "Socket.hpp"

namespace Networking::Sockets {
class SocketIPv4 : public Socket {
 public:
  typedef struct YStreamBuffer {
    size_t fBytesRead;
    std::stringstream fBufferData;
    YStreamBuffer() : fBytesRead(0) {}
  } YStreamBuffer;

  static SocketIPv4 CreateTcpSocket();
  ~SocketIPv4();
  bool Bind(const uint32_t address, const uint16_t port);
  bool Bind(const std::string& pAddress, const uint16_t port);

  // Optionally get client connection details
  void OnAccept(std::function<void(struct sockaddr_in&)> callback);
  void OnDataReceive(std::function<std::string(const std::string&)> dataHandler,
                     const std::string& terminator);
  void Run();
  YStreamBuffer HandleRead(const int connfd);

 protected:
 private:
  SocketIPv4(const int type);
  void GetConnectionDetails(const struct sockaddr_in);

  std::function<void(struct sockaddr_in&)> _acceptCallback;
  std::function<std::string(const std::string&)> _dataHandlerCb;
  std::string _terminator;
};

}  // namespace Wrappers
