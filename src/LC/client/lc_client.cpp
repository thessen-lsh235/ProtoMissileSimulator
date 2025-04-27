#include "lc_client.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

LcClient::LcClient(const std::string& ip, int port)
    : serverIp_(ip), serverPort_(port), sock_(-1)
{
}

LcClient::~LcClient()
{
    disconnect();
}

bool LcClient::connectToServer()
{
    sock_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_ < 0) {
        std::cerr << "소켓 생성 실패" << std::endl;
        return false;
    }

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(serverPort_);

    if (inet_pton(AF_INET, serverIp_.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cerr << "IP 변환 실패" << std::endl;
        return false;
    }

    if (connect(sock_, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "서버 연결 실패" << std::endl;
        return false;
    }

    return true;
}

bool LcClient::sendMessage(const std::string& message)
{
    if (sock_ < 0) return false;
    return send(sock_, message.c_str(), message.size(), 0) >= 0;
}

std::string LcClient::receiveMessage()
{
    if (sock_ < 0) return "";

    char buffer[1024] = {0};
    int len = recv(sock_, buffer, sizeof(buffer), 0);
    if (len > 0) {
        return std::string(buffer, len);
    }
    return "";
}

void LcClient::disconnect()
{
    if (sock_ >= 0) {
        close(sock_);
        sock_ = -1;
    }
}
