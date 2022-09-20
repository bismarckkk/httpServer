#include <iostream>

#include "tcpServer.h"

int main() {
    tcpServer server(8888, "0.0.0.0");
    server.start(1);

    while (1);
}
