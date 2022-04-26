#include <stdint.h>
#include <functional>

namespace Wrappers
{
  class Socket
  {
  public:
    ~Socket();
    static Socket* CreateSocket(const uint16_t family, const int type, const int protocol);
    static Socket* CreateSocketIPv4();
    static Socket* CreateSocketIPv6();
    bool Listen(const int numListeners);
    bool Bind(const uint32_t inAddr, const uint16_t port);
    bool Bind(const std::string& presentationAddress, const uint16_t port);
    bool Connect(const char *presentationAddress, const uint16_t port);
    void SetCallback(std::function<int(int, int)> callback);
    void RunCallback();
    const int GetSockFD(void) const;

  private:
    Socket(const int newSockFd, const uint16_t family);

    int _sockfd;
    unsigned short _family;
    std::function<int(int, int)> fCallback;
  };
}
