#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include <unordered_map>
#include <string>
#include <cstring>
#include <functional>
#include <random>
#include <sstream>
#include <iomanip>


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
    
    // Authentication methods
    bool registerUser(const std::string& username, const std::string& password);
    bool loginUser(const std::string& username, const std::string& password);
    bool userExists(const std::string& username) const;
    
private:
    void handleIncomingPacket(ClientSession& clientSession);
    void sendResponseToClient(const ClientSession& clientSession, const char* response);
    void broadcastMessage(const char* message, const ClientSession& senderSession);
    
    // Helper methods for password hashing
    std::string hashPassword(const std::string& password, const std::string& salt = "");
    std::string generateSalt();
    bool verifyPassword(const std::string& password, const std::string& hashedPassword);

    ServerSocket serverSocket; // Server socket for receiving client messages
    std::unordered_map<std::string, ClientSession> clientSessions; // Map of active client sessions indexed by host:port
    std::unordered_map<std::string, std::string> userCredentials; // Map of username -> salted hashed password
    MessageStore messageStore; // Store for chat messages (for reliable delivery and retransmission)
    bool running_; // Flag to control the server's main loop
    char buffer[MAX_BUFFER_SIZE]; // Buffer for receiving messages from clients
    // TODO: add thread pool for handling client requests
};

#endif // CHAT_SERVER_H
