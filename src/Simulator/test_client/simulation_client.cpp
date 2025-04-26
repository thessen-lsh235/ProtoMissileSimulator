#include "missileInfo.h"
#include "targetInfo.h"
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

    // Target 명령 생성 (고정된 위치)
    TargetInfo target;
    std::strncpy(target.name, "alpha", sizeof(target.name) - 1); // 이름 설정
    target.name[sizeof(target.name) - 1] = '\0';                 // null-terminate
    target.pos_x = 1500.0;                                       // 고정된 x 좌표
    target.pos_y = 1500.0;                                       // 고정된 y 좌표
    target.speed = 0;                                            // 속도 0 (고정)
    target.degree = 0.0;                                         // 이동 없음

    // TargetInfo를 바이트 배열로 직렬화
    std::vector<uint8_t> target_serialized = target.serializeImpl();

    // Target 명령 전송
    ssize_t sent = sendto(sockfd, target_serialized.data(), target_serialized.size(), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (sent < 0) {
        perror("sendto failed");
        close(sockfd);
        return 1;
    }
    std::cout << "표적 명령 전송 완료: Name=" << target.name << ", Pos=(" << target.pos_x << ", " << target.pos_y
              << "), Speed=" << target.speed << ", Degree=" << target.degree << std::endl;

    // MissileInfo 생성
    MissileInfo missile;
    missile.missile_id = 1;    // 미사일 ID
    missile.LS_pos_x = 1000.0; // 발사대 x 좌표
    missile.LS_pos_y = 1000.0; // 발사대 y 좌표
    missile.speed = 300;       // 미사일 속도

    // 미사일 각도 계산 (Target의 고정 위치로 이동)
    missile.degree = atan2(target.pos_y - missile.LS_pos_y, target.pos_x - missile.LS_pos_x) * 180.0 / M_PI;

    // MissileInfo를 바이트 배열로 직렬화
    std::vector<uint8_t> missile_serialized = missile.serializeImpl();

    // Missile 명령 전송
    sent = sendto(sockfd, missile_serialized.data(), missile_serialized.size(), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
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