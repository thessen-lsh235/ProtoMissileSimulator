#include "Entities.h"
#include "EntityFactory.h"
#include "UDPServer.h"
#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

#include "missile.h"
#include "target.h"

void registerUdpHandlers(UDPServer &server, std::vector<std::unique_ptr<Entity>> &entities) {
    server.setMessageHandler([&entities](const std::vector<uint8_t> &received_data) {
        try {
            // 데이터 타입 확인 및 처리
            uint8_t type = received_data[0];
            if (static_cast<DataType>(type) == DataType::Missile) {
                MissileInfo missile;
                missile.deserializeImpl(received_data);
                std::cout << "Received MissileInfo - missile_id: " << missile.missile_id << ", LS_pos_x: " << missile.LS_pos_x
                          << ", LS_pos_y: " << missile.LS_pos_y << ", Speed: " << missile.speed << ", Degree: " << missile.degree
                          << std::endl;
            } else if (static_cast<DataType>(type) == DataType::Target) {
                TargetInfo target;
                target.deserializeImpl(received_data);
                std::cout << "Received TargetInfo - ID: " << target.id << ", Pos_x: " << target.pos_x << ", Pos_y: " << target.pos_y
                          << ", Speed: " << target.speed << ", Degree: " << target.degree << std::endl;
            } else {
                std::cerr << "Unknown data type received: " << static_cast<int>(type) << std::endl;
            }
        } catch (const std::exception &e) {
            std::cerr << "[UDP] 명령 처리 중 오류 발생: " << e.what() << std::endl;
        }
    });
}

int main() {
    // UDP 서버 시작
    UDPServer server(9000);
    std::atomic<bool> server_running(true);

    // 엔티티 컨테이너
    std::vector<std::unique_ptr<Entity>> entities;

    // 명령 수신 시 엔티티 동적 생성 핸들러 등록
    registerUdpHandlers(server, entities);

    std::thread server_thread([&]() {
        server.start();
        server_running = false;
    });

    // 서버 스레드 종료 대기
    if (server_thread.joinable()) {
        server_thread.join();
    }

    return 0;
}
