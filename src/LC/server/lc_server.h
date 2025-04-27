#pragma once

#include <string>

class LcServer
{
public:
    LcServer(int port);
    ~LcServer();

    bool startServer();
    std::string receiveMessage();
    void stopServer();

private:
    int serverFd_;
    int clientFd_;
    int port_;
};
