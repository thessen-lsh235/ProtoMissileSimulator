#pragma once
#include <functional>
#include <string>

class UDPServer {
public:
    using MessageHandler = std::function<void(const std::string&)>;
    UDPServer(unsigned short port);
    ~UDPServer();
    void start();
    void setMessageHandler(MessageHandler handler);
private:
    unsigned short port;
    int server_socket;
    MessageHandler messageHandler;
};