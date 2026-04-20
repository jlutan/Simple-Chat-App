#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include <unordered_map>
#include <string>
#include <cstring>

#include "client_session.h"
#include "../transport/udp_socket.h"
#include "../common/definitions.h"
#include "message_store.h"

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 8080
#define CONNECT_MESSAGE "Hello, Client! This is the Server.\0"

class ChatServer {
public:
    ChatServer() : running_(false) {}
    ~ChatServer() {
        if (running_) {
            stop();
        }
    }

    bool start(const char* serverIp, const unsigned short port);
    void run();
    void stop();
private:
    void handleIncomingPacket(ClientSession& clientSession);
    void sendResponseToClient(const ClientSession& clientSession, const char* response);
    void broadcastMessage(const char* message, const ClientSession& senderSession);

    ServerSocket serverSocket_; // Server socket for receiving client messages
    uint32_t session_count_ = 0; // Counter to assign unique connection IDs to clients
    std::unordered_map<std::string, ClientSession> clientSessions_; // Map of active client sessions indexed by host:port
    MessageStore messageStore_; // Store for chat messages (for reliable delivery and retransmission)
    bool running_; // Flag to control the server's main loop
    char buffer_[MAX_BUFFER_SIZE]; // Buffer for receiving messages from clients
    // TODO: add thread pool for handling client requests
};

#endif // CHAT_SERVER_H
