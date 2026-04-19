// #include <iostream>
#include <cstdio>

#include "chat_server.h"


bool ChatServer::start(unsigned short port) {
    // Bind the server socket to the specified port
    if (serverSocket.bind(port)) {
        return true;
    } else {
        return false;
    }
}

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
    close(serverSocket.getSocketFd());
}

void ChatServer::handleIncomingPacket(ClientSession& clientSession) {  
    ssize_t bytesReceived = serverSocket.recvFrom(clientSession.addr, 
        reinterpret_cast<uint8_t*>(buffer), MAX_BUFFER_SIZE);

    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; // Null-terminate the received data
        printf("Received message from client %s:%d: %s\n", 
            inet_ntoa(clientSession.addr.sin_addr), ntohs(clientSession.addr.sin_port), buffer);
        printf("Bytes received: %zd\n", bytesReceived);
    } else {
        perror("Failed to receive message from client.\n");
    }
}

void ChatServer::sendResponseToClient(const ClientSession& clientSession, const char* response) {
    if (!serverSocket.sendTo(clientSession.addr, 
        reinterpret_cast<const uint8_t*>(response), strlen(response))) { 
        perror("Error sending response to client.\n");
    } else {
        printf("Response sent to client.\n");
    }
}

void ChatServer::broadcastMessage(const char* message, const ClientSession& senderSession) {
    for (const auto& sessionPair : clientSessions) {
        const ClientSession& clientSession = sessionPair.second;
        if (clientSession.connectionId != senderSession.connectionId) { // Don't send to the sender
            sendResponseToClient(clientSession, message);
        }
    }
}
