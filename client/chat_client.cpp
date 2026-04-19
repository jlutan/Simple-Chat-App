#include "chat_client.h"

bool ChatClient::connectToServer(const char* serverIp, unsigned short serverPort) {
    return clientSocket.setRemoteServer(serverIp, serverPort);
}

bool ChatClient::sendBroadcast(const char* message) {
    return clientSocket.sendToRemoteServer(reinterpret_cast<const uint8_t*>(message), strlen(message));
}

/**
 * Receives a message from the server and stores it in the provided buffer.
 * The sender's IP address and port are stored in the serverAddr parameter.
 * The buffer is null-terminated after receiving the message.
 * Returns true if a message was received successfully, false otherwise.
 */
bool ChatClient::receiveMessage(char* buffer, size_t bufferSize) {
    sockaddr_in serverAddr = clientSocket.getRemoteServerAddress();
    ssize_t bytesReceived = clientSocket.recvFrom(serverAddr, 
        reinterpret_cast<uint8_t*>(buffer), bufferSize);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; // Null-terminate the received message
        strcpy(this->buffer, buffer); // Store the received message in the client's buffer
        return true;
    } else {
        return false;
    }
}
