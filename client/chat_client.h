#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

#include <cstring>
#include <cstdint>

#include "../transport/udp_socket.h" // For ClientSocket
#include "../common/definitions.h" // For MAX_BUFFER_SIZE

class ChatClient {
public:
    ChatClient() {}
    ~ChatClient() {}
    bool connectToServer(const char* serverIp, unsigned short serverPort);
    bool sendBroadcast(const char* message);
    bool sendMessage(const char* username, const char* message);
    bool receiveMessage(char* buffer, size_t bufferSize);
    const char* getLastReceivedMessage() const { return buffer; }
private:
    ClientSocket clientSocket; // Client socket for sending messages to and receiving responses
    char buffer[MAX_BUFFER_SIZE]; // Buffer for receiving messages from the server
};

#endif // CHAT_CLIENT_H
