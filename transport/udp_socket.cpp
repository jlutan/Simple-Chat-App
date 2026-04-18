#include "udp_socket.h"

bool UdpSocket::bind(unsigned short port) {
    // socket creation and binding to the specified port
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        return false;
    }
    
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);
    
    if (::bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        return false;
    }
    
    return true;
}