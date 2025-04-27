// missile_command_client.cpp
// 미사일 발사 명령을 시뮬레이터 서버로 전송하는 테스트 클라이언트
#include "missileInfo.h"
#include <arpa/inet.h>
#include <cmath>
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

    // Target의 예상 위치 계산 (10초 뒤)
    double target_x = 100.0 + 50 * 10 * cos(90.0 * M_PI / 180.0); // x 좌표
    double target_y = 200.0 + 50 * 10 * sin(90.0 * M_PI / 180.0); // y 좌표

    // MissileInfo 생성
    MissileInfo missile;
    missile.missile_id = 1;   // 미사일 ID
    missile.LS_pos_x = 50.0;  // 발사대 x 좌표
    missile.LS_pos_y = 100.0; // 발사대 y 좌표
    missile.speed = 300;      // 미사일 속도

    // 미사일 각도 계산
    missile.degree = atan2(target_y - missile.LS_pos_y, target_x - missile.LS_pos_x) * 180.0 / M_PI;

    // MissileInfo를 바이트 배열로 직렬화
    std::vector<uint8_t> serialized_cmd = missile.serializeImpl();

    // 직렬화된 데이터를 전송
    ssize_t sent = sendto(sockfd, serialized_cmd.data(), serialized_cmd.size(), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (sent < 0) {
        perror("sendto failed");
        close(sockfd);
        return 1;
    }
    std::cout << "미사일 발사 명령 전송 완료: ID=" << missile.missile_id << ", LS_pos=(" << missile.LS_pos_x << ", " << missile.LS_pos_y
              << "), Speed=" << missile.speed << ", Degree=" << missile.degree << std::endl;

    close(sockfd);
    return 0;
}
