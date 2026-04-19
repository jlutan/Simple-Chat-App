#include <iostream>
#include <cstring>

#include "../transport/udp_socket.h"
#include "chat_client.h"
#include "../common/packet.h"

int main() {
    const char* message = "Hello, Server! This is the Client.\0";

    std::cout << "Client is running..." << std::endl;
    
    ClientSocket socket = ClientSocket(SERVER_ADDRESS, SERVER_PORT); // Create a client socket instance

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

    // TODO: Begin Handshake request to the server
    Packet handshakePacket;
    handshakePacket.header.type = PacketType::CONNECT;
    handshakePacket.header.payloadLength = 0;

    return 0;
}