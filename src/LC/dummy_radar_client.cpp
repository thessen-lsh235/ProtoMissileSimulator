#include "../common/target.h"   // TargetInfo
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "[RadarClient] 소켓 생성 실패" << std::endl;
        return -1;
    }

    struct sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000); // LC 서버 포트
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        std::cerr << "[RadarClient] IP 주소 변환 실패" << std::endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "[RadarClient] 서버 연결 실패" << std::endl;
        close(sock);
        return -1;
    }

    std::cout << "[RadarClient] 발사통제기(LC) 서버 연결 성공" << std::endl;

    int target_id = 0;

    while (true) {
        TargetInfo target;
        target.id = target_id++;             // 매번 다른 id
        target.pos_x = 100.0 + target_id;     // x좌표 업데이트
        target.pos_y = 200.0 + target_id;     // y좌표 업데이트
        target.speed = 250;                   // 속도
        target.degree = 45.0;                 // 각도

        std::vector<uint8_t> serialized = target.serializeImpl();

        ssize_t sent = send(sock, serialized.data(), serialized.size(), 0);
        if (sent < 0) {
            std::cerr << "[RadarClient] 데이터 전송 실패" << std::endl;
            break;
        } else {
            std::cout << "[RadarClient] TargetInfo 전송 완료 (id=" << target.id << ")" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1)); // 1초 대기
    }

    close(sock);
    return 0;
}
