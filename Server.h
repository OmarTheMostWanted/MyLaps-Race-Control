//
// Created by tmw on 6/20/24.
//

#ifndef MYLAPS_SERVER_H
#define MYLAPS_SERVER_H


#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include "Race.h"

class Server {
private:
    int server_fd;
    struct sockaddr_in address;
    Race race;

private:
    std::string getRaceData() {
        std::ostringstream ss;
        ss << "Ranking of drivers:\n";
        for (const auto& driver : race.drivers) {
            ss << "Driver " << driver.number
               << " Best Lap: " << std::fixed << std::setprecision(2) << driver.bestLap.count() / 100.0 << "s"
               << " Average Lap: " << driver.averageLap.count() / 100.0 << "s\n";
        }
        return ss.str();
    }

    std::string getAverageLapTime(int driverNumber) {
        auto it = std::find_if(race.drivers.begin(), race.drivers.end(), [&](const Driver& d) { return d.number == driverNumber; });
        if (it != race.drivers.end()) {
            std::ostringstream ss;
            ss << "Driver " << it->number << " Average Lap: " << std::fixed << std::setprecision(2) << it->averageLap.count() / 100.0 << "s\n";
            return ss.str();
        } else {
            return "Driver not found\n";
        }
    }

public:
    Server(int port) {
        int opt = 1;
        int addrlen = sizeof(address);

        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        // Forcefully attaching socket to the port
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        // Bind the socket to the network address and port
        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        if (listen(server_fd, 3) < 0) {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        // Read race data and calculate best and average lap times
        race.readData("../TestData/karttimes.csv");
        race.calculateBestLap();
        race.calculateAverageLap();

    }

    void run() {
        while (true) {
            std::cout << "Waiting for a connection...\n";
            int new_socket;
            int addrlen = sizeof(address);
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            std::cout << "Connected to a client.\n";

            // Communication with the client
            char buffer[1024] = {0};
            read(new_socket, buffer, 1024);
            std::string message(buffer);
            std::cout << "Message from client: " << message << std::endl;

            if (message.rfind("average lap time:", 0) == 0) {
                int driverNumber = std::stoi(message.substr(17));
                std::string response = getAverageLapTime(driverNumber);
                send(new_socket, response.c_str(), response.size(), 0);
            } else if (message .rfind("race data", 0) == 0) {
                // Send race data to client
                std::string raceData = getRaceData();
                send(new_socket, raceData.c_str(), raceData.size(), 0);
            }

            close(new_socket);
        }
    }
};


#endif //MYLAPS_SERVER_H
