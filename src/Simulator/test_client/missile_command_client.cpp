// missile_command_client.cpp
// 미사일 발사 명령을 시뮬레이터 서버로 전송하는 테스트 클라이언트
#include "missile.h"
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

    // MissileInfo 생성
    MissileInfo missile;
    missile.missile_id = 1;   // 미사일 ID
    missile.LS_pos_x = 50.0;  // 발사대 x 좌표
    missile.LS_pos_y = 100.0; // 발사대 y 좌표
    missile.speed = 300;      // 미사일 속도
    missile.degree = 45.0;    // 발사각

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

    // 역직렬화 테스트
    MissileInfo deserialized_missile;
    deserialized_missile.deserializeImpl(serialized_cmd);
    std::cout << "역직렬화된 MissileInfo - missile_id: " << deserialized_missile.missile_id
              << ", LS_pos_x: " << deserialized_missile.LS_pos_x << ", LS_pos_y: " << deserialized_missile.LS_pos_y
              << ", Speed: " << deserialized_missile.speed << ", Degree: " << deserialized_missile.degree << std::endl;

    close(sockfd);
    return 0;
}
