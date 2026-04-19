#ifndef MESSAGE_STORE_H
#define MESSAGE_STORE_H

#include <cstdint>

#include "../common/packet.h"
#include "../common/definitions.h"

struct Message {
    uint32_t connectionId; // Unique identifier for the client session
    uint32_t sequenceNumber; // Sequence number for reliable message delivery
    uint8_t payload[MAX_BUFFER_SIZE]; // Message payload (up to 1024 bytes)
};

struct MessageStore {
    Message messages[MAX_MESSAGE_STORE_SIZE]; // Store up to 100 messages
    uint32_t messageCount = 0; // Number of messages currently stored
};

#endif // MESSAGE_STORE_H
