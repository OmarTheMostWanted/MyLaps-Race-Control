/**
 * @file Client.h
 * @brief This file contains the Client class which is used to connect to a server and send/receive messages.
 */

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

/**
 * @class Client
 * @brief This class represents a client that connects to a server and sends/receives messages.
 */
class Client {
private:
    int sock = 0;  ///< Socket descriptor for the client
    struct sockaddr_in serv_addr{};  ///< Server address structure
    std::mutex mtx;  ///< Mutex for synchronizing access to the socket

public:
    /**
     * @brief Constructs a new Client object.
     * @param serverIP The IP address of the server to connect to.
     * @param port The port number of the server to connect to.
     */
    Client(const std::string &serverIP, int port);

    /**
     * @brief Destructor for the Client object. Closes the socket connection.
     */
    ~Client();

    /**
     * @brief Connects the client to the server.
     */
    void connectToServer();

    /**
     * @brief Sends a request to the server for the average lap time of a specific driver.
     * @param driverNumber The number of the driver to request the average lap time for.
     */
    void requestAverageLapTime(int driverNumber);

    /**
     * @brief Sends a request to the server for race data.
     */
    void requestRaceData();

    /**
     * @brief Runs the client, allowing the user to send requests to the server and receive responses.
     */
    void run();

    /**
     * @brief Sends raw data to the server.
     * @param data The data to send.
     */
    void sendRawSocket(const std::string &data);

    /**
     * @brief Reads raw data from the server.
     * @param response The string to store the response in.
     * @param responsePattern The regex pattern to match the response against.
     * @param timeoutSeconds The timeout in seconds for the read operation.
     */
    void readRawSocket(std::string &response, const std::regex &responsePattern, unsigned int timeoutSeconds);
};

#endif //MYLAPS_CLIENT_H