#ifndef PACKET_H
#define PACKET_H

#include <cstdint>

#define MAX_BUFFER_SIZE 1024

enum class PacketType: uint8_t {
    CONNECT = 0,
    CONNECT_ACK = 1,
    CHAT = 2,
    CHAT_ACK = 3,
    HEARTBEAT = 4,
    DISCONNECT = 5,
    ERROR = 6
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
