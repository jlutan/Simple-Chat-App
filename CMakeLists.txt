cmake_minimum_required(VERSION 3.10)
project(networked_chat_app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(client_test 
    transport/udp_socket.cpp
    client/client_main.cpp)

add_executable(server_test 
    transport/udp_socket.cpp
    server/server_main.cpp)