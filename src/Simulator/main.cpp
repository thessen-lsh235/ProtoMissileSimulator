#include "Entities.h"
#include "UDPServer.h"
#include <atomic>
#include <cmath>
#include <fmt/core.h>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "missile.h"
#include "missileInfo.h" // 추가
#include "target.h"
#include "targetInfo.h" // 추가

// 엔티티 컨테이너와 동기화를 위한 mutex
std::mutex entities_mutex;
std::vector<std::unique_ptr<Entity>> entities; // 엔티티 컨테이너

// 충돌 감지 함수
bool detectCollision(const Missile &missile, const Target &target) {
    double dx = missile.getX() - target.getX();
    double dy = missile.getY() - target.getY();
    return (std::abs(dx) <= 100.0 && std::abs(dy) <= 100.0);
}

// 관리 쓰레드 함수
void collisionManagerThread(std::atomic<bool> &running) {
    int elapsed_time = 0; // 경과 시간 (초 단위)
    while (running) {
        {
            std::lock_guard<std::mutex> lock(entities_mutex);
            fmt::print("========================================\n");
            fmt::print("현재 엔티티 개수: {}\n", entities.size());
            fmt::print("경과 시간: {}초\n", elapsed_time);
            fmt::print("========================================\n");

            // 충돌 감지 및 처리
            for (auto it_missile = entities.begin(); it_missile != entities.end();) {
                auto missile = dynamic_cast<Missile *>(it_missile->get());
                if (missile == nullptr) {
                    ++it_missile;
                    continue;
                }

                bool collision_detected = false;

                for (auto it_target = entities.begin(); it_target != entities.end();) {
                    auto target = dynamic_cast<Target *>(it_target->get());
                    if (target == nullptr) {
                        ++it_target;
                        continue;
                    }

                    if (detectCollision(*missile, *target)) {
                        fmt::print("\n*** 충돌 발생! ***\n");
                        fmt::print("미사일 ID: [{}], 위치: (x: {:.2f}, y: {:.2f})\n", missile->getId(), missile->getX(), missile->getY());
                        fmt::print("표적 이름: [{}], 위치: (x: {:.2f}, y: {:.2f})\n", target->getName(), target->getX(), target->getY());
                        fmt::print("미사일과 표적이 제거됩니다.\n");
                        fmt::print("******************************\n\n");

                        it_target = entities.erase(it_target); // Target 제거
                        collision_detected = true;
                    } else {
                        ++it_target;
                    }
                }

                if (collision_detected) {
                    fmt::print("Removing missile ID: {}\n", missile->getId());
                    it_missile = entities.erase(it_missile); // Missile 제거
                } else {
                    ++it_missile;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 1초 대기
        ++elapsed_time;                                       // 경과 시간 증가
    }
}

// 개별 엔티티를 처리하는 쓰레드 함수
void entitySimulationThread(Entity *entity, std::atomic<bool> &running) {
    while (running) {
        try {
            entity->updatePosition(0.1); // 0.1초마다 위치 업데이트

            // 현재 위치 출력
            if (auto missile = dynamic_cast<Missile *>(entity)) {
                fmt::print("[Missile] ID: {}, x: {:.2f}, y: {:.2f}\n", missile->getId(), missile->getX(), missile->getY());
            } else if (auto target = dynamic_cast<Target *>(entity)) {
                fmt::print("[Target] Name: {}, x: {:.2f}, y: {:.2f}\n", target->getName(), target->getX(), target->getY());
            }
        } catch (const std::exception &e) {
            fmt::print(stderr, "Entity removed: {}\n", e.what());
            break; // 쓰레드 종료
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 0.1초 대기
    }
}

// UDP 핸들러 등록 함수
void registerUdpHandlers(UDPServer &server, std::vector<std::thread> &entity_threads, std::atomic<bool> &running) {
    server.setMessageHandler([&entity_threads, &running](const std::vector<uint8_t> &received_data) {
        try {
            uint8_t type = received_data[0];
            if (static_cast<DataType>(type) == DataType::Missile) {
                MissileInfo missile_info;
                missile_info.deserializeImpl(received_data);

                fmt::print("Received MissileInfo - missile_id: {}, LS_pos_x: {}, LS_pos_y: {}, Speed: {}, Degree: {}\n",
                           missile_info.missile_id, missile_info.LS_pos_x, missile_info.LS_pos_y, missile_info.speed, missile_info.degree);

                // Missile 객체 생성 및 추가
                auto missile = std::make_unique<Missile>(missile_info.missile_id, missile_info.LS_pos_x, missile_info.LS_pos_y,
                                                         missile_info.speed, missile_info.degree);
                {
                    std::lock_guard<std::mutex> lock(entities_mutex);
                    entities.push_back(std::move(missile));
                }
                entity_threads.emplace_back(
                    [missile = entities.back().get(), &running]() mutable { entitySimulationThread(std::move(missile), running); });
                entity_threads.back().detach(); // 쓰레드 분리
            } else if (static_cast<DataType>(type) == DataType::Target) {
                TargetInfo target_info;
                target_info.deserializeImpl(received_data);

                fmt::print("Received TargetInfo - Name: {}, Pos_x: {}, Pos_y: {}, Speed: {}, Degree: {}\n", target_info.name,
                           target_info.pos_x, target_info.pos_y, target_info.speed, target_info.degree);

                // Target 객체 생성 및 추가
                auto target =
                    std::make_unique<Target>(target_info.name, target_info.pos_x, target_info.pos_y, target_info.speed, target_info.degree);
                {
                    std::lock_guard<std::mutex> lock(entities_mutex);
                    entities.push_back(std::move(target));
                }
                entity_threads.emplace_back(
                    [target = entities.back().get(), &running]() mutable { entitySimulationThread(std::move(target), running); });
                entity_threads.back().detach(); // 쓰레드 분리
            } else {
                fmt::print(stderr, "Unknown data type received: {}\n", static_cast<int>(type));
            }
        } catch (const std::exception &e) {
            fmt::print(stderr, "[UDP] 명령 처리 중 오류 발생: {}\n", e.what());
        }
    });
}

int main() {
    // UDP 서버 시작
    UDPServer server(9000);
    std::atomic<bool> server_running(true);

    // 엔티티 쓰레드 컨테이너
    std::vector<std::thread> entity_threads;

    // 충돌 관리 쓰레드 시작
    std::thread collision_manager_thread(collisionManagerThread, std::ref(server_running));

    // 명령 수신 시 엔티티 동적 생성 핸들러 등록
    registerUdpHandlers(server, entity_threads, server_running);

    // 서버 쓰레드 시작
    std::thread server_thread([&]() {
        server.start();
        server_running = false;
    });

    // 서버 쓰레드 종료 대기
    if (server_thread.joinable()) {
        server_thread.join();
    }

    // 서버 종료 시 모든 쓰레드 종료
    server_running = false;
    if (collision_manager_thread.joinable()) {
        collision_manager_thread.join();
    }

    return 0;
}
