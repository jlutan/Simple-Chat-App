#include <cstdio>
#include <cstring>

#include <sys/socket.h>

#include "client_session.h"
#include "chat_server.h"

int main() {
    // Server initialization
    ChatServer server;
    if (!server.start(SERVER_ADDRESS, SERVER_PORT)) {
        perror("Failed to start the chat server.\n");
        return 1;
    }
    printf("ChatServer is running and listening on port %d.\n", SERVER_PORT);

    server.run();   // Start the server's main loop to handle client connections and messages
    server.stop();  // TODO: run in a separate thread and add signal handling for graceful shutdown

    return 0;
}