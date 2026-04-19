#ifndef CLIENT_SESSION_H
#define CLIENT_SESSION_H

#include <cstdint>
#include <string>
#include <chrono>

#include <netinet/in.h> // For sockaddr_in

#include "../transport/udp_socket.h" // For ClientSocket
#include "../common/definitions.h"

/** @brief Structure representing a client state */
struct ClientSession {
    uint32_t connectionId; // Unique identifier assigned after connection handshake
    ClientSocket clientSocket; // Client socket for sending responses back to the client
    sockaddr_in addr;
    std::string username;
    std::chrono::steady_clock::time_point lastActiveTime; // Timestamp of the last activity from this client
    bool connected; 
    uint32_t nextExpectedSeq; // Sequence number for reliable message delivery
};

#endif // CLIENT_SESSION_H
