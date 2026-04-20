#ifndef RELIABLE_SESSION_H
#define RELIABLE_SESSION_H

#include "../common/packet.h" // For Packet, PacketType
#include "udp_socket.h"

#define ISN_RANGE 1000
enum class SessionState {
    CLOSED = 0,
    SYN_SENT = 1,
    ESTABLISHED = 2,
    FIN_WAIT1 = 3,
    FIN_WAIT2 = 4,
    TIME_WAIT = 5
};

bool sendSYN(UdpSocket& socket, uint32_t& connectionId, uint32_t& sequenceNumber);
bool sendACK(UdpSocket& socket, uint32_t connectionId, uint32_t ackNumber);
bool sendSYNACK(UdpSocket& socket, uint32_t connectionId, uint32_t sequenceNumber, uint32_t ackNumber);


#endif // RELIABLE_SESSION_H
