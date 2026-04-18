#include "udp_socket.h"
#include <iostream>

int main() {
    UdpSocket socket;
    if (socket.bind(12345)) {
        std::cout << "Socket bound successfully to port 12345." << std::endl;
    } else {
        std::cerr << "Failed to bind socket to port 12345." << std::endl;
        return 1;
    }
    
    // Additional test code for sending and receiving data would go here
    // sockaddr_in senderIp;
    // uint8_t buffer[1024];
    // ssize_t receivedBytes = socket.recvFrom(senderIp, buffer, sizeof(buffer));
    // if (receivedBytes > 0) {
    //     std::cout << "Received " << receivedBytes << " bytes from " << inet_ntoa(senderIp.sin_addr) << ":" << ntohs(senderIp.sin_port) << std::endl;
    // }
    
    return 0;
}