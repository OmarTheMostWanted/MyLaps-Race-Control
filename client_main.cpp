//
// Created by tmw on 6/20/24.
//

#include "Client.h"


int main() {
    Client client("127.0.0.1", 8080);
    client.connectToServer();

    client.requestRaceData();
    std::string response = client.receiveMessage();
    std::cout << "Response from server: " << response << std::endl;


//    client.connectToServer();
//    client.requestAverageLapTime(1); // Request average lap time of driver 1
//    response = client.receiveMessage();
//    std::cout << "Response from server: " << response << std::endl;
    return 0;
}