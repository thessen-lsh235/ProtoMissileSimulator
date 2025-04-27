#pragma once

#include <string>

class LcClient
{
public:
    LcClient(const std::string& ip, int port);
    ~LcClient();

    bool connectToServer();
    bool sendMessage(const std::string& message);
    std::string receiveMessage();
    void disconnect();

private:
    std::string serverIp_;
    int serverPort_;
    int sock_;
};