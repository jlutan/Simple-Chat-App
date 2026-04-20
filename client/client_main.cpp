#include <iostream>
#include <cstring>

// #include "../transport/udp_socket.h"
#include "chat_client.h"
#include "../common/packet.h"

int main() {
    const char* message = "Hello, Server! This is the Client.\0";
    
    ChatClient client = ChatClient(); 
    if (!client.connectToServer(SERVER_ADDRESS, SERVER_PORT)) {
        std::cerr << "Failed to connect to server." << std::endl;
        return 1;
    }

    std::cout << "Client is running..." << std::endl;

    // Send broadcast message to the server
    if (client.sendBroadcast(message)) {
        std::cout << "Broadcast message sent to server." << std::endl;
        printf("Bytes sent: %zu\n", strlen(message));
    } else {
        std::cerr << "Failed to send broadcast message to server." << std::endl;
    }

    // Receive response from the server
    char responseBuffer[MAX_BUFFER_SIZE];
    ssize_t bytesReceived = client.receiveMessage(
        responseBuffer, sizeof(responseBuffer)); // Receive response from the server
    if (bytesReceived > 0) {
        std::cout << "Received response from server: " << responseBuffer << std::endl;
    } else {
        std::cerr << "Failed to receive response from server." << std::endl;
    }

    return 0;
}

void sendHandshakeRequest(ChatClient& client) {
    

    
}