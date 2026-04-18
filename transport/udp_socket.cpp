#include "udp_socket.h"

UdpSocket::UdpSocket() {
    // Create a UDP socket using IPv4 protocol
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        perror("UdpSocket constructor - socket creation failed\n");
        UdpSocket::~UdpSocket(); // Call destructor to clean up if socket creation fails
    }
}

// Constructor that accepts an existing socket file descriptor
UdpSocket::UdpSocket(int socket_fd) {
    if (socket_fd < 0) {
        perror("UdpSocket constructor - invalid socket file descriptor\n");
        this->socket_fd = -1; // Set to -1 to indicate an invalid socket
        UdpSocket::~UdpSocket();
    } else {
        this->socket_fd = socket_fd;
    }
}

UdpSocket::~UdpSocket() {
    if (socket_fd >= 0) {
        printf("Closing UDP socket with file descriptor %d\n", socket_fd);
        close(socket_fd); // close the socket when the object is destroyed
    }
}

/**
 * Sends data to the specified destination IP address and port.
 * Returns true if the data was sent successfully, false otherwise.
 */
bool UdpSocket::sendTo(const sockaddr_in& destIp, const uint8_t* data, size_t size) {
    // send data to the specified destination IP address and port
    if (socket_fd < 0) {
        return false; // socket not initialized
    }
    // use sendto() system call (GNU C library) to send data over UDP
    ssize_t sentBytes = sendto(socket_fd, data, size, 0, (struct sockaddr*)&destIp, sizeof(destIp));
    return sentBytes == static_cast<ssize_t>(size);
}

/**
 * Blocks and waits for data to be received from any sender.
 * Receives data from any sender and stores the sender's IP address and port in senderIp.
 * The received data is stored in the provided buffer and data size in bufferSize.
 * Returns the number of bytes received, or -1 on error.
 */
ssize_t UdpSocket::recvFrom(sockaddr_in& senderIp, uint8_t* buffer, size_t bufferSize) {
    // receive data from any sender and store the sender's IP address and port
    if (socket_fd < 0) {
        return -1; // socket not initialized
    }
    
    socklen_t senderAddrLen = sizeof(senderIp);
    ssize_t receivedBytes = recvfrom(socket_fd, buffer, bufferSize, 0, (struct sockaddr*)&senderIp, &senderAddrLen);
    return receivedBytes; // returns number of bytes received or -1 on error
}



// ServerSocket method implementations

/**
 * Binds the UDP socket to the specified port using IPv4 protocol.
 * Accepts messages from local host 127.0.0.1
 * Returns true if the socket was bound successfully, false otherwise.
 */
bool ServerSocket::bind(unsigned short port) {
    // socket creation and binding to the specified port
    // create DGRAM (UDP) socket using IPv4 (AF_INET) protocol
    if (socket_fd < 0) {
        perror("UdpSocket::bind - socket creation failed");
        return false;
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    server_addr.sin_port = htons(port); // convert port number to network byte order
    
    if (::bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        return false;
    }
    
    return true;
}



// ClientSocket method implementations

/**
 * Sets the remote server's IP address and port for the client socket.
 * Returns true if the remote server information was set successfully, false otherwise.
 */
bool ClientSocket::setRemoteServer(const char* serverIp, unsigned short serverPort) {
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(serverPort); // convert port number to network byte order
    if (inet_pton(AF_INET, serverIp, &remote_addr.sin_addr) <= 0) {
        return false; // invalid IP address format
    }
    return true;
}

bool ClientSocket::sendToRemoteServer(const uint8_t* data, size_t size) {
    return sendTo(remote_addr, data, size);
}