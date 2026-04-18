#include <cstdint>
#include <netinet/in.h> // For sockaddr_in

#define MAX_BUFFER_SIZE 1024


struct ClientSession {
    uint32_t clientSocketId;
    sockaddr_in clientAddress;
    char buffer[MAX_BUFFER_SIZE];
    // Additional session-related data can be added here (e.g., last active time, message queue, etc.)
};