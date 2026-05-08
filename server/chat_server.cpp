// #include <iostream>
#include <cstdio>
#include <crypt.h>
#include <random>
#include <string>

#include "chat_server.h"
#include "client_session.h"


/**
 * Starts the chat server and binds it to the specified port.
 * @param port The port number to bind the server socket to.
 * @return true if the server starts successfully, false otherwise.
 */
bool ChatServer::start(unsigned short port) {
    // Bind the server socket to the specified port
    if (serverSocket.bind(port)) {
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
        // Update client session information
        std::string clientKey = inet_ntoa(clientSession.addr.sin_addr) + std::to_string(ntohs(clientSession.addr.sin_port));
        if (clientSessions.find(clientKey) == clientSessions.end()) {
            clientSession.connectionId = clientSessions.size() + 1; // Assign a unique connection ID
            clientSession.connected = true;
            clientSession.lastActiveTime = std::chrono::steady_clock::now();
            clientSessions[clientKey] = clientSession; // Store the client session in the map
            printf("New client connected: %s:%d (Connection ID: %u)\n", 
                inet_ntoa(clientSession.addr.sin_addr), ntohs(clientSession.addr.sin_port), clientSession.connectionId);
        } else {
            // Update existing client session's last active time
            clientSessions[clientKey].lastActiveTime = std::chrono::steady_clock::now();
        }

        // Send response back to the client
        sendResponseToClient(clientSession, static_cast<const char*>(CONNECT_MESSAGE)); 
    }
}

void ChatServer::stop() {
    running_ = false;
    close(serverSocket.getSocketFd());
}

/**
 * Blocks and waits for an incoming message from a client, receives the message, and updates the client session information.
 * The received message is stored in the server's buffer.
 */
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

/**
 * Generates a random salt for bcrypt hashing.
 * Bcrypt salt format: $2b$<cost>$<22-character-salt>
 * @return A bcrypt-compatible salt string.
 */
std::string ChatServer::generateSalt() {
    const char* bcrypt_chars = "./ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 63);
    
    std::string salt;
    salt += '$';
    salt += '2';      // bcrypt version
    salt += 'b';
    salt += '$';
    salt += '1';      // cost factor: 10
    salt += '0';
    salt += '$';
    
    for (int i = 0; i < 22; ++i) {
        salt += bcrypt_chars[dis(gen)];
    }
    
    return salt;
}

/**
 * Hashes a password using bcrypt algorithm.
 * @param password The plaintext password to hash.
 * @param salt The salt to use for hashing. If empty, a new salt is generated.
 * @return A bcrypt hash string.
 */
std::string ChatServer::hashPassword(const std::string& password, const std::string& salt) {
    std::string actualSalt = salt.empty() ? generateSalt() : salt;
    
    struct crypt_data data;
    data.initialized = 0;
    
    char* result = crypt_r(password.c_str(), actualSalt.c_str(), &data);
    if (result == nullptr) {
        printf("Error: bcrypt hashing failed\n");
        return "";
    }
    
    return std::string(result);
}

/**
 * Verifies a plaintext password against a stored bcrypt hash.
 * @param password The plaintext password to verify.
 * @param hashedPassword The stored bcrypt hash.
 * @return true if the password matches, false otherwise.
 */
bool ChatServer::verifyPassword(const std::string& password, const std::string& hashedPassword) {
    struct crypt_data data;
    data.initialized = 0;
    
    char* result = crypt_r(password.c_str(), hashedPassword.c_str(), &data);
    if (result == nullptr) {
        return false;
    }
    
    return std::string(result) == hashedPassword;
}

/**
 * Registers a new user with username and password.
 * @param username The username for the new account.
 * @param password The plaintext password for the new account.
 * @return true if registration is successful, false if username already exists.
 */
bool ChatServer::registerUser(const std::string& username, const std::string& password) {
    if (userCredentials.find(username) != userCredentials.end()) {
        printf("Registration failed: Username '%s' already exists.\n", username.c_str());
        return false;
    }
    
    if (username.empty() || password.empty()) {
        printf("Registration failed: Username and password cannot be empty.\n");
        return false;
    }
    
    // Hash the password and store it
    std::string hashedPassword = hashPassword(password);
    userCredentials[username] = hashedPassword;
    printf("User '%s' registered successfully.\n", username.c_str());
    return true;
}

/**
 * Authenticates a user by verifying username and password.
 * @param username The username to authenticate.
 * @param password The plaintext password to verify.
 * @return true if authentication is successful, false otherwise.
 */
bool ChatServer::loginUser(const std::string& username, const std::string& password) {
    auto it = userCredentials.find(username);
    if (it == userCredentials.end()) {
        printf("Login failed: Username '%s' not found.\n", username.c_str());
        return false;
    }
    
    if (!verifyPassword(password, it->second)) {
        printf("Login failed: Invalid password for user '%s'.\n", username.c_str());
        return false;
    }
    
    printf("User '%s' logged in successfully.\n", username.c_str());
    return true;
}

/**
 * Checks if a user exists in the credentials database.
 * @param username The username to check.
 * @return true if the user exists, false otherwise.
 */
bool ChatServer::userExists(const std::string& username) const {
    return userCredentials.find(username) != userCredentials.end();
}
