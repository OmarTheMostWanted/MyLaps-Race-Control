//
// Created by tmw on 6/20/24.
//

#ifndef MYLAPS_CLIENT_H
#define MYLAPS_CLIENT_H


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

class Client {
private:
    int sock = 0;
    struct sockaddr_in serv_addr;

public:
    Client(const std::string& serverIP, int port) {
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            std::cout << "Socket creation error\n";
            exit(EXIT_FAILURE);
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);

        // Convert IPv4 and IPv6 addresses from text to binary form
        if (inet_pton(AF_INET, serverIP.c_str(), &serv_addr.sin_addr) <= 0) {
            std::cout << "Invalid address/ Address not supported\n";
            exit(EXIT_FAILURE);
        }
    }

    void connectToServer() {
        if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
            std::cout << "Connection Failed\n";
            exit(EXIT_FAILURE);
        }
    }

    void sendMessage(const std::string& message) {
        send(sock, message.c_str(), message.size(), 0);
    }

    std::string receiveMessage() {
        char buffer[1024] = {0};
        read(sock, buffer, 1024);
        return std::string(buffer);
    }

    void requestAverageLapTime(int driverNumber) {
        std::string message = "average lap time:" + std::to_string(driverNumber);
        sendMessage(message);
    }

    void requestRaceData() {
        std::string message = "race data";
        sendMessage(message);
    }

};

#endif //MYLAPS_CLIENT_H
