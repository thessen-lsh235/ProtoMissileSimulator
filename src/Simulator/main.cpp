#include "Entities.h"
#include "EntityFactory.h"
#include "UDPServer.h"
#include "common.h"
#include "util.h" // get_val 함수 사용을 위해 포함
#include <vector>
#include <thread>
#include <atomic>
#include <iostream>

// ---------- 유틸리티 함수 제거 ----------
// get_val 함수는 util.cpp로 이동되었습니다.

void registerUdpHandlers(UDPServer& server, std::vector<std::unique_ptr<Entity>>& entities) {
    server.setMessageHandler([&entities](const std::string& msg) {
        try {
            if (msg.find("\"type\":\"missile\"") != std::string::npos) {
                Missile missile = Missile::fromString(msg);
                entities.push_back(EntityFactory::createMissile(missile.name, {missile.pos[0], missile.pos[1], missile.pos[2]}, {missile.vel[0], missile.vel[1], missile.vel[2]}));
                std::cout << "[UDP] 미사일 생성: " << missile.name << std::endl;
            } else if (msg.find("\"type\":\"target\"") != std::string::npos) {
                Target target = Target::fromString(msg);
                entities.push_back(EntityFactory::createTarget(target.name, {target.pos[0], target.pos[1], target.pos[2]}, {target.vel[0], target.vel[1], target.vel[2]}));
                std::cout << "[UDP] 표적 생성: " << target.name << std::endl;
            } else {
                std::cerr << "[UDP] 알 수 없는 명령 유형: " << msg << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "[UDP] 명령 처리 중 오류 발생: " << e.what() << " | 메시지: " << msg << std::endl;
        }
    });
}

int main()
{
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
