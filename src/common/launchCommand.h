#pragma once

#include <cstdint>

#pragma pack(push, 1)
struct LaunchCommand {
    int launcher_id;    // 발사대 ID
    int missile_id;     // 미사일 ID
    double launch_angle; // 발사각
};
#pragma pack(pop)
