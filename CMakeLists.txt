cmake_minimum_required(VERSION 3.28)
project(MyLaps)

set(CMAKE_CXX_STANDARD 23)

add_executable(MyLaps main.cpp
        Race.cpp
        Race.h
        Server.cpp
        Server.h
        Client.cpp
        Client.h
)


add_executable(client client_main.cpp
        Race.cpp
        Race.h
        Client.cpp
        Client.h
        client_main.cpp
)
