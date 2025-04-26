#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include "missile.h"  // MissileInfo 포함

int main() {
    // 1. 발사 정보 설정
    MissileInfo missile;
    missile.missile_id = 1;
    missile.LS_pos_x = 176431.680592;
    missile.LS_pos_y = 140854.998730;
    missile.speed = 400;          // 예시 속도
    missile.degree = 123.44;          // 진북 기준 발사각

    // 2. 직렬화 (바이너리 변환)
    std::vector<uint8_t> packet = missile.toBytes();
    
    // 3. UDP 전송
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket creation failed");
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9000);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    ssize_t sent = sendto(sock, packet.data(), packet.size(), 0,
                          (sockaddr*)&server_addr, sizeof(server_addr));
    if (sent < 0) {
        perror("sendto failed");
    } else {
        std::cout << "[Missile Fire Command is sent]\n";
    }

    close(sock);
    return 0;
}
