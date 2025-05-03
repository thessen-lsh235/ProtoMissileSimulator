#pragma once
#include <vector>
#include <string>
#include <stdexcept>

enum class OperationMode {
    ENGAGEMENT,   // 교전 모드
    MOVEMENT,     // 이동 모드
    STOP          // 정지 모드
};

struct LauncherConfig {
    int id = 0;                  // 발사대 ID
    long long x = 0;            // 경도 (Longitude)
    long long y = 0;             // 위도 (Latitude)
    int missile_count = 0;       // 가용 미사일 개수
    std::vector<int> missile_ids; // 미사일 ID 리스트
    OperationMode mode = OperationMode::STOP; // 기본은 정지 모드

    // 문자열을 모드(enum)로 변환
    static OperationMode stringToMode(const std::string& modeStr) {
        if (modeStr == "ENGAGEMENT") return OperationMode::ENGAGEMENT;
        else if (modeStr == "MOVEMENT") return OperationMode::MOVEMENT;
        else if (modeStr == "STOP") return OperationMode::STOP;
        else throw std::invalid_argument("Unknown OperationMode: " + modeStr);
    }

    // 모드를 문자열로 변환
    static std::string modeToString(OperationMode mode) {
        switch (mode) {
            case OperationMode::ENGAGEMENT: return "ENGAGEMENT";
            case OperationMode::MOVEMENT: return "MOVEMENT";
            case OperationMode::STOP: return "STOP";
            default: return "UNKNOWN";
        }
    }
};
