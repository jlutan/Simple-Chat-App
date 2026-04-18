#include <cstdint>

enum class PacketType: uint8_t {
    ChatMessage,
    UserJoined,
    UserLeft,
    // Add more packet types as needed
};