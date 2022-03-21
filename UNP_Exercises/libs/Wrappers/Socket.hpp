namespace Wrappers
{
  class Socket
  {
  public:
    ~Socket();
    static Socket *CreateSocket(const int family, const int type, const int protocol);
    int Listen();
    int Bind(const uint8_t addressFamily, const uint32_t inAddr, const uint16_t port);
    int Connect(const int addressFamily, const char *address, const unsigned short port);
    
    int GetSockFD(void);

  private:
    Socket(const int newSockFd);

    int sockfd;
  };
}
