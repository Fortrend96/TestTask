#include "TcpServer.h"
#include "ClientSession.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <memory>
#include <mutex>

TcpServer::TcpServer(uint16_t port) : port_(port), server_fd_(-1) {}
TcpServer::~TcpServer() {
    if (server_fd_ != -1) close(server_fd_);
}

void TcpServer::start() {
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd_ < 0) throw std::runtime_error("socket() failed");

    int opt = 1;
    setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port_);

    if (bind(server_fd_, (sockaddr*)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("bind() failed");

    if (listen(server_fd_, 128) < 0)
        throw std::runtime_error("listen() failed");

    std::cout << "Server listening on 0.0.0.0:" << port_ << std::endl;

    std::vector<std::unique_ptr<ClientSession>> sessions;
    std::mutex sessions_mtx;

    while (true) {
        sockaddr_in cli_addr{};
        socklen_t len = sizeof(cli_addr);
        int client_fd = accept(server_fd_, (sockaddr*)&cli_addr, &len);
        if (client_fd < 0) {
            std::perror("accept");
            continue;
        }

        char ipbuf[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &cli_addr.sin_addr, ipbuf, sizeof(ipbuf));
        std::cout << "Accepted connection from " << ipbuf << ":" << ntohs(cli_addr.sin_port) << std::endl;

        auto session = std::make_unique<ClientSession>(client_fd);
        session->start();

        {
            std::lock_guard<std::mutex> lock(sessions_mtx);
            sessions.push_back(std::move(session));
        }

    }
}
