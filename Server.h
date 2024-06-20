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
#include <thread>
#include <mutex>
#include <future>
#include <regex>
#include <chrono>
#include "Race.h"

#define BUFFER_SIZE 1024
#define TIMEOUT 300

#ifdef DEBUG
#define TRACE(m) std::cout << __func__ << " " << m << std::endl;
#else
#define TRACE(m)
#endif

/**
 * @class Server
 * @brief This class represents a server that handles client connections and processes race data.
 */
class Server {
private:
    int server_fd;  ///< File descriptor for the server socket
    struct sockaddr_in address{};  ///< Address structure for the server
    Race race{};  ///< Race object to process race data

private:
    /**
     * @brief Sends raw data to the client socket.
     * @param clientSocket The client socket to send data to.
     * @param data The data to send.
     * @param mtx Mutex for synchronizing access to the socket.
     */
    void sendRawSocket(int clientSocket, const std::string &data, std::mutex& mtx);

    /**
     * @brief Reads raw data from the client socket.
     * @param clientSocket The client socket to read data from.
     * @param response The string to store the response in.
     * @param responsePattern The regex pattern to match the response against.
     * @param timeoutSeconds The timeout in seconds for the read operation.
     * @param mtx Mutex for synchronizing access to the socket.
     */
    void readRawSocket(int clientSocket, std::string &response, const std::regex &responsePattern, unsigned int timeoutSeconds, std::mutex& mtx);

    /**
     * @brief Handles a client connection.
     * @param clientSocket The client socket to handle.
     */
    void handleClient(int clientSocket);

public:
    /**
     * @brief Constructs a new Server object.
     * @param port The port number to bind the server to.
     */
    Server(int port);

    /**
     * @brief Runs the server, accepting client connections and handling them.
     */
    void run();

};


#endif //MYLAPS_SERVER_H