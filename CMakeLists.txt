cmake_minimum_required(VERSION 3.10)
project(networked_chat_app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(server server/server_main.cpp)
add_executable(client client/client_main.cpp)