// target_command_client.cpp
// 표적 명령을 시뮬레이터 서버로 전송하는 테스트 클라이언트
#include "targetInfo.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <vector>

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
    TargetInfo target;
    std::cout << "Enter target name: ";
    std::cin.getline(target.name, sizeof(target.name));
    target.pos_x = 100.0; // 초기 x 좌표
    target.pos_y = 200.0; // 초기 y 좌표
    target.speed = 50;    // 속도
    target.degree = 90.0; // 이동 각도

    // TargetInfo를 바이트 배열로 직렬화
    std::vector<uint8_t> serialized_cmd = target.serializeImpl();

    // 직렬화된 데이터를 전송
    ssize_t sent = sendto(sockfd, serialized_cmd.data(), serialized_cmd.size(), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (sent < 0) {
        perror("sendto failed");
        close(sockfd);
        return 1;
    }
    std::cout << "표적 명령 전송 완료: Name=" << target.name << ", Pos=(" << target.pos_x << ", " << target.pos_y
              << "), Speed=" << target.speed << ", Degree=" << target.degree << std::endl;

    close(sockfd);
    return 0;
}
