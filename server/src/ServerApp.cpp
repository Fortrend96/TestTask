#include "ServerApp.h"
#include <iostream>
#include "TcpServer.h"

void ServerApp::run(uint16_t port) {
    try {
        TcpServer server(port);
        server.start();
    } catch (const std::exception& ex) {
        std::cerr << "Server error: " << ex.what() << std::endl;
    }
}
