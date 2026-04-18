#include <cstdint>
#include <netinet/in.h> // system socket API; For sockaddr_in

class UdpSocket {
public:
    bool bind(unsigned short port);
    bool sendTo(const sockaddr_in& destIp, const uint8_t* data, size_t size);
    ssize_t recvFrom(sockaddr_in& senderIp, uint8_t* buffer, size_t bufferSize);
};