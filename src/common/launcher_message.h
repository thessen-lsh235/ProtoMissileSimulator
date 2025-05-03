#pragma once
#include <cstdint>
#include "launchCommand.h"
#include "launcher_config.h" // for OperationMode

enum class CommandType : uint8_t {
    LAUNCH = 1,
    MOVE = 2,
    MODE_CHANGE = 3,
    STATUS_REQUEST = 4
};

#pragma pack(push, 1)
struct LauncherMessage {
    CommandType type;
    union {
        LaunchCommand launch;
        struct {
            long long new_x;
            long long new_y;
        } move;
        struct {
            OperationMode new_mode;
        } mode_change;
        struct {
            int dummy;
        } status_request;
    };
};
#pragma pack(pop)

#pragma pack(push, 1)
struct LauncherStatusMessage {
    int id;
    long long x, y; // 경도, 위도
    int missile_count;
    int missile_ids[10];  // 최대 10개로 고정
    OperationMode mode;
};
#pragma pack(pop)