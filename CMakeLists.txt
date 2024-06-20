cmake_minimum_required(VERSION 3.28)
project(MyLaps)

set(CMAKE_CXX_STANDARD 23)

add_executable(server main.cpp
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


# Add the preprocessor definitions
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(server PRIVATE DEBUG)
endif()

if(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(server PRIVATE RELEASE)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(client PRIVATE DEBUG)
endif()

if(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(client PRIVATE RELEASE)
endif()