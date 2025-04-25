// target_command_client.cpp
// 표적 명령을 시뮬레이터 서버로 전송하는 테스트 클라이언트
#include "common.h"
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9000

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        return 1;
    }

    struct sockaddr_in servaddr{};
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &servaddr.sin_addr);

    // Target 명령 생성
    Target target;
    target.name = "Target1";
    target.pos[0] = 100.0f;
    target.pos[1] = 200.0f;
    target.pos[2] = 300.0f;
    target.vel[0] = 0.0f;
    target.vel[1] = 0.0f;
    target.vel[2] = 0.0f;

    // 명령 직렬화 후 전송
    std::string serialized_cmd = target.toString();
    ssize_t sent = sendto(sockfd, serialized_cmd.c_str(), serialized_cmd.size(), 0,
                          (struct sockaddr*)&servaddr, sizeof(servaddr));
    if (sent < 0) {
        perror("sendto failed");
        close(sockfd);
        return 1;
    }
    std::cout << "표적 명령 전송 완료: " << serialized_cmd << std::endl;

    close(sockfd);
    return 0;
}
