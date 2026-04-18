#include <cstdint>

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
    uint16_t payloadSize;
    uint32_t sequenceNumber;
    uint32_t connectionId;
    uint32_t ackNumber;
};