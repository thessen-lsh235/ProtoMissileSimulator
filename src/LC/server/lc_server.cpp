#include "lc_server.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

LcServer::LcServer(int port)
    : serverFd_(-1), clientFd_(-1), port_(port)
{
}

LcServer::~LcServer()
{
    stopServer();
}

bool LcServer::startServer()
{
    serverFd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd_ < 0) {
        std::cerr << "[Server] 소켓 생성 실패" << std::endl;
        return false;
    }

    int opt = 1;
    setsockopt(serverFd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port_);

    if (bind(serverFd_, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "[Server] 바인드 실패" << std::endl;
        return false;
    }

    if (listen(serverFd_, 1) < 0) {
        std::cerr << "[Server] 리슨 실패" << std::endl;
        return false;
    }

    std::cout << "[Server] 클라이언트 접속 대기 중..." << std::endl;

    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);
    clientFd_ = accept(serverFd_, (struct sockaddr*)&client_addr, &client_len);
    if (clientFd_ < 0) {
        std::cerr << "[Server] 클라이언트 연결 실패" << std::endl;
        return false;
    }

    std::cout << "[Server] 클라이언트 연결 완료" << std::endl;

    return true;
}

std::string LcServer::receiveMessage()
{
    if (clientFd_ < 0) return "";

    char buffer[1024] = {0};
    int len = recv(clientFd_, buffer, sizeof(buffer), 0);
    if (len > 0) {
        return std::string(buffer, len);
    }
    return "";
}

void LcServer::stopServer()
{
    if (clientFd_ >= 0) {
        close(clientFd_);
        clientFd_ = -1;
    }
    if (serverFd_ >= 0) {
        close(serverFd_);
        serverFd_ = -1;
    }
}
