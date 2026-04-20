#include "chat_client.h"


bool ChatClient::connectToServer(const char* serverIp, unsigned short serverPort) {
    
    if (!clientSocket.setRemoteServer(serverIp, serverPort)) {
        return false;
    }

    bool ackReceived = false;
    const int maxRetries = 5;
    int retryCount = 0;

    while (!ackReceived && retryCount < maxRetries) {
        sendHandshakeRequest(); // Send handshake request to the server

        char responseBuffer[MAX_BUFFER_SIZE];
        ssize_t bytesReceived = receiveMessage(responseBuffer, sizeof(responseBuffer)); // Wait for response from the server
        if (bytesReceived > 0) {
            // Check if the response is a CONNECT_ACK
            Packet* responsePacket = reinterpret_cast<Packet*>(responseBuffer);
            if (responsePacket->header.type == PacketType::SYN_ACK) {
                ackReceived = true;
                printf("Received CONNECT_ACK from server.\n");
                sendAck(*responsePacket);
            } else {
                printf("Received unexpected packet type from server: %d\n", static_cast<int>(responsePacket->header.type));
            }
        } else {
            printf("No response received from server, retrying... (%d/%d)\n", retryCount + 1, maxRetries);
        }
        retryCount++;
    }

}

bool ChatClient::sendToServer(const char* message) {
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

bool ChatClient::sendHandshakeRequest() {
    Packet handshakePacket;
    handshakePacket.header.type = PacketType::SYN;
    handshakePacket.header.payloadLength = 0;
    handshakePacket.header.sequenceNumber = rand() % ISN_RANGE;
    handshakePacket.header.version = PACKET_VER;

    return clientSocket.sendToRemoteServer(
        reinterpret_cast<const uint8_t*>(&handshakePacket), sizeof(Packet));
}

bool ChatClient::sendAck(Packet &receivedPacket) {
    Packet ackPacket;
    ackPacket.header.type = PacketType::ACK;
    ackPacket.header.payloadLength = 0;
    ackPacket.header.ackNumber = receivedPacket.header.sequenceNumber + 1;
    ackPacket.header.version = PACKET_VER;
    ackPacket.header.connectionId = receivedPacket.header.connectionId; 

    return clientSocket.sendToRemoteServer(
        reinterpret_cast<const uint8_t*>(&ackPacket), sizeof(Packet));
}