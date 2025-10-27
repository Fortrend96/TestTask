#pragma once
#include <atomic>
#include <thread>
#include <string>
#include "IntQueue.h"

class ClientSession
{
public:
    explicit ClientSession(int client_fd);
    ~ClientSession();

    void start();
    void join();

private:
    void run();
    bool sendAll(const std::string& data);

private:
    int client_fd_;
    std::thread th_;
    std::atomic<bool> running_;
    IntQueue queue_;
};
