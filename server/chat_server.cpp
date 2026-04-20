// #include <iostream>
#include <cstdio>

#include "chat_server.h"

/**
 * Starts the chat server and binds it to the specified port.
 * @param port The port number to bind the server socket to.
 * @return true if the server starts successfully, false otherwise.
 */
bool ChatServer::start(const char* serverIp, const unsigned short port) {
    // Bind the server socket to the specified port
    if (serverSocket_.bind(serverIp, port)) {
        return true;
    } else {
        return false;
    }
}

/**
 * Begins server main loop listening for incoming client connections.
 */
void ChatServer::run() {
    running_ = true;
    
    // Main loop to handle incoming client messages and manage sessions
    while (running_) {
        ClientSession clientSession;
        // blocking call to receive incoming messages from clients
        handleIncomingPacket(clientSession);
        // Send response back to the client
        sendResponseToClient(clientSession, static_cast<const char*>(CONNECT_MESSAGE)); 
        
    }
}

void ChatServer::stop() {
    running_ = false;
    close(serverSocket_.getSocketFd());
}

/**
 * Blocks and waits for an incoming message from a client, receives the message, and updates the client session information.
 * The received message is stored in the server's buffer.
 */
void ChatServer::handleIncomingPacket(ClientSession& clientSession) {  
    ssize_t bytesReceived = serverSocket_.recvFrom(clientSession.addr, 
        reinterpret_cast<uint8_t*>(buffer_), MAX_BUFFER_SIZE);

    if (bytesReceived > 0) {
        buffer_[bytesReceived] = '\0'; // Null-terminate the received data
        printf("Received message from client %s:%d: %s\n", 
            inet_ntoa(clientSession.addr.sin_addr), ntohs(clientSession.addr.sin_port), buffer_);
        printf("Bytes received: %zd\n", bytesReceived);
    } else {
        perror("Failed to receive message from client.\n");
    }
}

void ChatServer::sendResponseToClient(const ClientSession& clientSession, const char* response) {
    if (!serverSocket_.sendTo(clientSession.addr, 
        reinterpret_cast<const uint8_t*>(response), strlen(response))) { 
        perror("Error sending response to client.\n");
    } else {
        printf("Response sent to client.\n");
    }
}

void ChatServer::broadcastMessage(const char* message, const ClientSession& senderSession) {
    for (const auto& sessionPair : clientSessions_) {
        const ClientSession& clientSession = sessionPair.second;
        if (clientSession.connectionId != senderSession.connectionId) { // Don't send to the sender
            sendResponseToClient(clientSession, message);
        }
    }
}
