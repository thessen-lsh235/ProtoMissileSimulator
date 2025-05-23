#include "missileInfo.h"
#include "targetInfo.h"
#include <iostream>
#include <vector>

int main() {
    // Missile 객체 생성 및 직렬화
    MissileInfo missile;
    missile.missile_id = 1;
    missile.LS_pos_x = 10.5;
    missile.LS_pos_y = 20.5;
    missile.speed = 300;
    missile.degree = 45.0;

    std::vector<uint8_t> serialized_missile = missile.serializeImpl();
    std::cout << "Serialized Missile: ";
    for (uint8_t byte : serialized_missile) {
        std::cout << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;

    // 직렬화된 데이터를 역직렬화
    MissileInfo deserialized_missile;
    deserialized_missile.deserializeImpl(serialized_missile);
    std::cout << "Deserialized Missile - missile_id: " << deserialized_missile.missile_id << ", LS_pos_x: " << deserialized_missile.LS_pos_x
              << ", LS_pos_y: " << deserialized_missile.LS_pos_y << ", Speed: " << deserialized_missile.speed
              << ", Degree: " << deserialized_missile.degree << std::endl;

    // Target 객체 생성 및 직렬화
    TargetInfo target;
    target.pos_x = 100.0;
    target.pos_y = 200.0;
    target.speed = 50;
    target.degree = 90.0;

    std::vector<uint8_t> serialized_target = target.serializeImpl();
    std::cout << "Serialized Target: ";
    for (uint8_t byte : serialized_target) {
        std::cout << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;

    // 직렬화된 데이터를 역직렬화
    TargetInfo deserialized_target;
    deserialized_target.deserializeImpl(serialized_target);
    std::cout << "Deserialized Target - Pos_x: " << deserialized_target.pos_x << ", Pos_y: " << deserialized_target.pos_y
              << ", Speed: " << deserialized_target.speed << ", Degree: " << deserialized_target.degree << std::endl;

    return 0;
}