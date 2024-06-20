#include <iostream>
#include "Server.h"
#include <functional>
#include <string>

//extern "C" void printMessageInBinary();

int main(int argc, char* argv[]) {

    // password_hint() { return password_hint() }
    if (argc > 1) {
        auto s = std::string(argv[1]);
        std::hash<std::string> hashFunc;
        unsigned long hashValue = hashFunc(s);
        unsigned long expectedHash = 14655196254117661989;
        if (hashValue == expectedHash) {
            std::cout << "Correct password!" << std::endl;
            //printMessageInBinary(); // couldn't get this to work in time X(
            return 0;
        }

    } else {
        Server server(8080);
        server.run();
        return 0;
    }
}