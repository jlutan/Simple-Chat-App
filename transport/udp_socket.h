#include <cstdint>
#include <cstdio>
#include <cstring>

// UNIX/Linux system headers for socket programming
#include <netinet/in.h> // system socket API; For sockaddr_in
#include <unistd.h>     // for close() system call
#include <sys/socket.h> // for socket functions (socket(), bind(), sendto(), recvfrom())
#include <arpa/inet.h>  // for inet_pton() to convert IP address from text to binary form


#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 8080


class UdpSocket {
public:
    UdpSocket();
    UdpSocket(int socket_fd);
    ~UdpSocket();

    bool sendTo(const sockaddr_in& destIp, const uint8_t* data, size_t size);
    ssize_t recvFrom(sockaddr_in& senderIp, uint8_t* buffer, size_t bufferSize);
    int getSocketFd() const { return socket_fd; } 
protected:
    int socket_fd; // file descriptor for the UDP socket
};

class ServerSocket : public UdpSocket {
public:
    ServerSocket(): UdpSocket() {}
    
    bool bind(unsigned short port);
    sockaddr_in getServerAddress() const { return server_addr; } 
private:
    sockaddr_in server_addr; // structure to hold server address information
};

class ClientSocket : public UdpSocket {
public:
    ClientSocket() : UdpSocket() {}
    ClientSocket(const char* serverIp, unsigned short serverPort) : UdpSocket() {
        setRemoteServer(serverIp, serverPort);
    }

    bool setRemoteServer(const char* serverIp, unsigned short serverPort);
    sockaddr_in getRemoteServerAddress() const { return remote_addr; }
    bool sendToRemoteServer(const uint8_t* data, size_t size);
private:
    sockaddr_in remote_addr; // structure to hold remote server address information
};
