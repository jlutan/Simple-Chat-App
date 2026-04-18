#include <iostream>
#include <sys/socket.h>

#include "client_session.h"
#include "../transport/udp_socket.h"


int main() {
    const char* replyMessage = "Hello, Client! This is the Server.";
    
    std::cout << "Server is running..." << std::endl;
    // Server initialization
    ServerSocket socket;
    if (socket.bind(SERVER_PORT)) {
        std::cout << "Server is running and bound to port " << SERVER_PORT << "." << std::endl;
    } else {
        std::cerr << "Failed to bind server socket to port " << SERVER_PORT << "." << std::endl;
        return 1;
    }


    // Receive data from client and print it
    ClientSession clientSession;
    char* buffer = clientSession.buffer;


    ssize_t bytesReceived = socket.recvFrom(clientSession.clientAddress, 
        reinterpret_cast<uint8_t*>(buffer), sizeof(buffer));
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; // Null-terminate the received data
        std::cout << "Received message from client: " << buffer << std::endl;
        printf("Bytes received: %zd\n", bytesReceived);
    } else {
        std::cerr << "Failed to receive message from client." << std::endl;
    }

    if (!socket.sendTo(clientSession.clientAddress, reinterpret_cast<const uint8_t*>(replyMessage), 
    strlen(replyMessage))) { // Send response back to the client
        std::cerr << "Failed to send response to client." << std::endl;
    } else {
        std::cout << "Reply message sent to client." << std::endl;
    }

    return 0;
}