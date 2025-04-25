// missile_command_client.cpp
// 미사일 발사 명령을 시뮬레이터 서버로 전송하는 테스트 클라이언트
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

    // 예시 미사일 발사 명령 (프로토콜에 맞게 수정 필요)
    const char* missile_cmd = "FIRE_MISSILE:target_id=1;x=100;y=200";

    ssize_t sent = sendto(sockfd, missile_cmd, strlen(missile_cmd), 0,
                          (struct sockaddr*)&servaddr, sizeof(servaddr));
    if (sent < 0) {
        perror("sendto failed");
        close(sockfd);
        return 1;
    }
    std::cout << "미사일 발사 명령 전송 완료: " << missile_cmd << std::endl;

    close(sockfd);
    return 0;
}
