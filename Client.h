#ifndef MYLAPS_CLIENT_H
#define MYLAPS_CLIENT_H


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <future>
#include <regex>
#include <chrono>
#include <thread>
#include <mutex>


#define BUFFER_SIZE 1024
#define TIMEOUT 300

#ifdef DEBUG
#define TRACE(m) std::cout << __func__ << " " << m << std::endl;
#else
#define TRACE(m)
#endif


class Client {
private:
    int sock = 0;
    struct sockaddr_in serv_addr{};
    std::mutex mtx;  // Mutex for synchronizing access to the socket

public:
    Client(const std::string &serverIP, int port) {
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            std::cerr << "Socket creation error\n";
            exit(EXIT_FAILURE);
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);

        // Convert IPv4 and IPv6 addresses from text to binary form
        if (inet_pton(AF_INET, serverIP.c_str(), &serv_addr.sin_addr) <= 0) {
            std::cerr << "Invalid address/ Address not supported\n";
            exit(EXIT_FAILURE);
        }
    }

    ~Client() {
        close(sock);
    }

    void connectToServer() {
        if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            std::cerr << "Connection Failed\n";
            exit(EXIT_FAILURE);
        }
    }

    void requestAverageLapTime(int driverNumber) {
        std::string message = "average lap time:" + std::to_string(driverNumber) + "\r\n";
        sendRawSocket(message);
    }

    void requestRaceData() {
        std::string message = "race data\r\n";
        sendRawSocket(message);
    }

    void run() {
        connectToServer();

        sendRawSocket("OK\r\n");
        std::cout << "Connected to server\n";

        std::string response;
        while (true) {
            std::cout << "1) Request race data\n2) Ask for average lap times for a specific kart\n3) Quit\n";
            std::string in;
            int option;

            std::cin >> in;

            try {
                option = std::stoi(in);
            } catch (const std::invalid_argument &e) {
                std::cerr << "Invalid option\n";
                continue;
            }

            switch (option) {
                case 1:
                    requestRaceData();
                    break;
                case 2:
                    std::cout << "Enter kart number: ";
                    int kartNumber;
                    std::cin >> in;

                    try {
                        kartNumber = std::stoi(in);
                    } catch (const std::invalid_argument &e) {
                        std::cerr << "Invalid kart number\n";
                        continue;
                    }

                    requestAverageLapTime(kartNumber);
                    break;
                case 3:
                    // cleanup
                    this->sendRawSocket("quit\r\n");
                    // close the socket
                    close(sock);
                    return;
                default:
                    std::cerr << "Invalid option\n";
                    continue;
            }

            std::thread([this, &response]() {
                try {
                    TRACE("Waiting for response");
                    readRawSocket(response, std::regex("(.*)OK\r\n"), TIMEOUT);
                    std::string data = response.substr(0, response.size() - 4);
                    std::cout << "Response: " << data << std::endl;
                    sendRawSocket("OK\r\n");
                    TRACE("OK sent")
                } catch (const std::exception &e) {
                    std::cerr << e.what() << '\n';
                }
            }).detach();

        }
    }

    void sendRawSocket(const std::string &data) {
        std::lock_guard<std::mutex> lock(mtx);  // Lock the mutex
        size_t sent = send(sock, data.c_str(), data.size(), 0);

        while (sent < data.size()) {
            sent += send(sock, data.c_str() + sent, data.size() - sent, 0);
        }

        // Mutex is automatically unlocked when lock goes out of scope
    }

    void readRawSocket(std::string &response, const std::regex &responsePattern, unsigned int timeoutSeconds) {
        std::lock_guard<std::mutex> lock(mtx);

        response.clear();
        char buffer[BUFFER_SIZE];

        auto future = std::async(std::launch::async, [&] {
            bool completed = false;
            do {
                memset(buffer, 0, BUFFER_SIZE);
                ssize_t r = recv(sock, buffer, BUFFER_SIZE - 1, 0);
                if (r < 0) {
                    throw std::runtime_error("Error reading response");
                }
                response += buffer;
                completed = std::regex_search(response, responsePattern);
            } while (!completed);
        });

        if (future.wait_for(std::chrono::seconds(timeoutSeconds)) == std::future_status::timeout) {
            throw std::runtime_error("Connection timed out while waiting for response");
        }
    }
};

#endif //MYLAPS_CLIENT_H