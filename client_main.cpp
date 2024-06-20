//
// Created by tmw on 6/20/24.
//

#include "Client.h"


int main() {
    Client client("127.0.0.1", 8080);
    client.run();

    return 0;
}