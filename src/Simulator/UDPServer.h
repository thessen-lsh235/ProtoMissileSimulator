#pragma once
#include <cstdint>
#include <functional>
#include <vector>

class UDPServer {
  public:
    using MessageHandler = std::function<void(const std::vector<uint8_t> &)>; // 핸들러 타입 수정

    explicit UDPServer(unsigned short port);
    ~UDPServer();

    void start();
    void setMessageHandler(MessageHandler handler);

  private:
    unsigned short port;
    int server_socket;
    MessageHandler messageHandler;
};