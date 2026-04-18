#include <iostream>

#include "../transport/udp_socket.h"
#include "chat_client.h"

int main() {
    const char* serverIp = SERVER_ADDRESS; // Server IP address (localhost)
    unsigned short serverPort = SERVER_PORT; // Server port number
    const char* message = "Hello, Server! This is the Client.";

    std::cout << "Client is running..." << std::endl;
    
    // Create socket and bind to any available port
    ClientSocket socket = ClientSocket(serverIp, serverPort); // Create a client socket instance

    // Send message to the server
    if (socket.sendToRemoteServer(reinterpret_cast<const uint8_t*>(message), strlen(message))) { 
        std::cout << "Hello message sent to server." << std::endl;
        printf("Bytes sent: %zu\n", strlen(message));
    } else {
        std::cerr << "Failed to send message to server." << std::endl;
    }

    // Receive response from the server
    uint8_t responseBuffer[MAX_BUFFER_SIZE];
    auto remoteAddr = socket.getRemoteServerAddress();
    ssize_t bytesReceived = socket.recvFrom(remoteAddr, 
        responseBuffer, sizeof(responseBuffer)); // Receive response from the server
    responseBuffer[bytesReceived] = '\0'; // Null-terminate the received response
    if (bytesReceived > 0) {
        std::cout << "Received response from server: " << responseBuffer << std::endl;
    } else {
        std::cerr << "Failed to receive response from server." << std::endl;
    }


    return 0;
}