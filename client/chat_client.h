#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

#include <cstring>
#include <cstdint>
#include <random>

#include "../transport/udp_socket.h" // For ClientSocket
#include "../common/definitions.h" // For MAX_BUFFER_SIZE
#include "../common/packet.h" // For Packet, PacketType
#include "../transport/reliable_session.h" // For SessionState

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 8080

class ChatClient {
public:
    ChatClient() {}
    ~ChatClient() {}
    bool connectToServer(const char* serverIp, unsigned short serverPort);
    bool sendToServer(const char* message);
    bool sendBroadcast(const char* message);
    bool sendDirectMessage(const char* username, const char* message);
    bool receiveMessage(char* buffer, size_t bufferSize);
    const char* getLastReceivedMessage() const { return buffer; }
private:
    bool sendHandshakeRequest();
    bool sendAck(Packet &receivedPacket);

    ClientSocket clientSocket; // Client socket for sending messages to and receiving responses
    char buffer[MAX_BUFFER_SIZE]; // Buffer for receiving messages from the server
};

#endif // CHAT_CLIENT_H
