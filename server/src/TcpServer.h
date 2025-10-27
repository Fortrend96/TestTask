#pragma once
#include <cstdint>

class TcpServer
{
public:
    explicit TcpServer(uint16_t port);
    ~TcpServer();

    void start();
private:
    uint16_t port_;
    int server_fd_;
};
