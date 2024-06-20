//
// Created by tmw on 6/20/24.
//

#include "Server.h"

void Server::sendRawSocket(int clientSocket, const std::string &data, std::mutex &mtx) {
    std::lock_guard<std::mutex> lock(mtx);  // Lock the mutex
    size_t sent = send(clientSocket, data.c_str(), data.size(), 0);

    while (sent < data.size()) {
        sent += send(clientSocket, data.c_str() + sent, data.size() - sent, 0);
    }

    // Mutex is automatically unlocked when lock goes out of scope
}

void Server::readRawSocket(int clientSocket, std::string &response, const std::regex &responsePattern, unsigned int timeoutSeconds, std::mutex &mtx) {
    std::lock_guard<std::mutex> lock(mtx);

    response.clear();
    char buffer[BUFFER_SIZE];

    auto future = std::async(std::launch::async, [&] {
        bool completed = false;
        do {
            memset(buffer, 0, BUFFER_SIZE);
            ssize_t r = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
            if (r < 0) {
                throw std::runtime_error("Error reading response");
            }
            response += buffer;
            completed = std::regex_search(response, responsePattern);
        } while (!completed);
    });

    if (future.wait_for(std::chrono::seconds(timeoutSeconds)) == std::future_status::timeout) {
        response = "quit\r\n";
    }
}

void Server::handleClient(int clientSocket) {
    std::mutex mtx;  // Mutex for synchronizing access to the socket
    try {
        std::string ack;
        TRACE("Waiting for client to send OK...")
        readRawSocket(clientSocket, ack, std::regex("OK\r\n"), TIMEOUT, mtx);
        if (ack != "OK\r\n") {
            std::cerr << "Client did not send OK on connection. Closing connection.\n";
            close(clientSocket);
            return;
        } else {
            std::cout << "Client connected.\n";
        }

        while (true) {
            std::string message;
            TRACE("Waiting for client message...")
            readRawSocket(clientSocket, message, std::regex("\r\n"), TIMEOUT, mtx); // Wait for client message
            TRACE("Received message: " << message)

            // response to message
            auto raceData = std::regex("race data\r\n");
            auto averageLapTime = std::regex("average lap time:([0-9]+)\r\n");
            auto quit = std::regex("quit\r\n");

            std::smatch match;
            if (std::regex_search(message, match, raceData)) {
                std::string response = race.getRaceData();
                TRACE("Sending response: " << response)
                sendRawSocket(clientSocket, response + "OK\r\n", mtx);
                TRACE("Response sent.")
            } else if (std::regex_search(message, match, averageLapTime)) {
                int driverNumber = std::stoi(match[1]);
                std::string response = race.getAverageLapTime(driverNumber);
                TRACE("Sending response: " << response)
                sendRawSocket(clientSocket, response + "OK\r\n", mtx);
                TRACE("Response sent.")
            } else if (std::regex_search(message, match, quit)) {
                std::cout << "Client requested to quit. Closing connection.\n";
                close(clientSocket);
                return;
            }

            // Wait for the client to acknowledge the response
            TRACE("Waiting for client to acknowledge response...")
            readRawSocket(clientSocket, ack, std::regex("OK\r\n"), TIMEOUT, mtx);

            if (ack != "OK\r\n") {
                std::cout << "Client did not send OK after response. Closing connection.\n";
                break;
            } else {
                TRACE("Client acknowledged response.")
            }
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << ". Closing connection.\n";
        close(clientSocket);
    }
}

Server::Server(int port) {
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
    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Read race data and calculate best and average lap times
    race.readData("../TestData/karttimes.csv"); //todo, remove hard coded path, read in a read async way, support multiple races
    race.calculateBestLap();
    race.calculateAverageLap();

}

void Server::run() {
    while (true) {
        std::cout << "Waiting for a connection...\n";
        int new_socket;
        int addrlen = sizeof(address);
        if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        TRACE("Connection accepted.")

        // Create a new thread for each client connection
        std::thread([this, new_socket]() {
            handleClient(new_socket);
        }).detach();
    }
}
