#ifndef PACKET_H
#define PACKET_H

#include <cstdint>

#define PACKET_VER 1
#define MAX_BUFFER_SIZE 1024

enum class PacketType: uint8_t {
    SYN = 0,
    ACK = 1,
    SYN_ACK = 2,
    CHAT = 3,
    CHAT_ACK = 4,
    HEARTBEAT = 5,
    DISCONNECT = 6,
    ERROR = 7
};

struct PacketHeader {
    PacketType  type;
    uint8_t     version;
    uint32_t connectionId;
    uint32_t sequenceNumber;
    uint32_t ackNumber;
    uint16_t payloadLength;
};

struct Packet {
    PacketHeader header;
    uint8_t payload[MAX_BUFFER_SIZE]; // Maximum payload size of 1024 bytes
};


#endif // PACKET_H
