cmake_minimum_required(VERSION 3.10)
project(networked_chat_app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(client 
    transport/udp_socket.cpp
    client/chat_client.cpp
    client/client_main.cpp)

add_executable(server 
    transport/udp_socket.cpp
    server/chat_server.cpp
    server/server_main.cpp)

# Link server with libcrypt for bcrypt support
target_link_libraries(server crypt)