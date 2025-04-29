#pragma once
#include <vector>
#include <string>
#include <stdexcept>

enum class OperationMode {
    ENGAGEMENT,   // 교전 모드
    MOVEMENT      // 이동 모드
};

struct LauncherConfig {
    int id = 0;                 // 발사대 ID
    double x = 0.0;             // 발사대 위치 X좌표
    double y = 0.0;             // 발사대 위치 Y좌표
    int missile_count = 0;       // 가용 미사일 개수
    std::vector<int> missile_ids; // 미사일 ID 리스트
    OperationMode mode = OperationMode::MOVEMENT; // 현재 모드
    bool is_faulty = false;      // 고장 여부 (true: 고장)

    // 문자열을 모드(enum)로 변환
    static OperationMode stringToMode(const std::string& modeStr) {
        if (modeStr == "ENGAGEMENT") return OperationMode::ENGAGEMENT;
        else if (modeStr == "MOVEMENT") return OperationMode::MOVEMENT;
        else throw std::invalid_argument("Unknown OperationMode: " + modeStr);
    }

    // 모드를 문자열로 변환 (Optional, 추가 기능)
    static std::string modeToString(OperationMode mode) {
        switch (mode) {
            case OperationMode::ENGAGEMENT: return "ENGAGEMENT";
            case OperationMode::MOVEMENT: return "MOVEMENT";
            default: return "UNKNOWN";
        }
    }
};
