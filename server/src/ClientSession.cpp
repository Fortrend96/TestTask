#include "ClientSession.h"
#include "Sorter.h"
#include <unistd.h>
#include <sys/socket.h>
#include <sstream>
#include <cstring>
#include <cctype>

static inline std::string trim(const std::string& s)
{
    size_t a = 0;

    while (a < s.size() && std::isspace((unsigned char)s[a]))
        ++a;

    size_t b = s.size();

    while (b > a && std::isspace((unsigned char)s[b-1]))
        --b;

    return s.substr(a, b-a);
}

ClientSession::ClientSession(int client_fd)
    : client_fd_(client_fd)
    , running_(false)
{}

ClientSession::~ClientSession()
{
    running_ = false;
    shutdown(client_fd_, SHUT_RDWR);
    if (th_.joinable()) th_.join();
    if (client_fd_ != -1) close(client_fd_);
}

void ClientSession::start()
{
    running_ = true;
    th_ = std::thread(&ClientSession::run, this);
}

void ClientSession::join()
{
    if (th_.joinable()) th_.join();
}

bool ClientSession::sendAll(const std::string& data)
{
    const char* p = data.data();
    size_t left = data.size();
    while (left) {
        ssize_t w = send(client_fd_, p, left, 0);
        if (w <= 0) return false;
        p += w;
        left -= w;
    }
    return true;
}

void ClientSession::run()
{
    std::string inbuf;
    char buf[512];
    while (running_)
    {
        ssize_t r = recv(client_fd_, buf, sizeof(buf), 0);
        if (r <= 0) break;
        inbuf.append(buf, buf + r);

        while (true)
        {
            auto pos = inbuf.find('\n');

            if (pos == std::string::npos)
                break;

            std::string line = trim(inbuf.substr(0, pos));
            inbuf.erase(0, pos + 1);
            if (line.empty())
                continue;

            if (line.size() >= 4 && line.rfind("ADD ", 0) == 0)
            {
                std::string args = line.substr(4);
                std::vector<int> nums;
                std::stringstream ss(args);
                std::string token;
                bool parse_ok = true;
                while (std::getline(ss, token, ',')) {
                    token = trim(token);
                    if (token.empty()) continue;
                    try { nums.push_back(std::stoi(token)); }
                    catch (...) { parse_ok = false; break; }
                }
                if (!parse_ok || nums.empty()) {
                    sendAll("ERR Bad number format\n");
                    continue;
                }
                for (int v : nums) queue_.push(v);
                sendAll("OK\n");

            } else if (line == "GET") {
                auto vec = queue_.to_vector();
                auto sorted = Sorter::mergeSort(vec);
                std::string out = "DATA ";
                for (size_t i = 0; i < sorted.size(); ++i) {
                    out += std::to_string(sorted[i]);
                    if (i + 1 < sorted.size()) out += ",";
                }
                out += "\n";
                sendAll(out);

            } else if (line == "CLEAR") {
                queue_.clear();
                sendAll("OK\n");

            } else {
                sendAll("ERR Unknown command\n");
            }
        }
    }
}
