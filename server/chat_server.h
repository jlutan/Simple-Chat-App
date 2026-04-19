#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include <unordered_map>
#include <string>
#include <cstring>

#include "client_session.h"
#include "../transport/udp_socket.h"
#include "../common/definitions.h"
#include "message_store.h"

#define CONNECT_MESSAGE "Hello, Client! This is the Server.\0"

class ChatServer {
public:
    ChatServer() : running_(false) {}
    ~ChatServer() {
        if (running_) {
            stop();
        }
    }
    
    bool start(unsigned short port);
    void run();
    void stop();
private:
    void handleIncomingPacket(ClientSession& clientSession);
    void sendResponseToClient(const ClientSession& clientSession, const char* response);
    void broadcastMessage(const char* message, const ClientSession& senderSession);

    ServerSocket serverSocket; // Server socket for receiving client messages
    std::unordered_map<std::string, ClientSession> clientSessions; // Map of active client sessions indexed by host:port
    MessageStore messageStore; // Store for chat messages (for reliable delivery and retransmission)
    bool running_; // Flag to control the server's main loop
    char buffer[MAX_BUFFER_SIZE]; // Buffer for receiving messages from clients
    // TODO: add thread pool for handling client requests
};

#endif // CHAT_SERVER_H
