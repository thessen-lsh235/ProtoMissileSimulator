#ifndef LAUNCH_PROTOCOL_H
#define LAUNCH_PROTOCOL_H

#include <cstring>

#pragma pack(push, 1) // 구조체 패딩 제거

struct LaunchCommand {
    char msg_type[8];        // 예: "LAUNCH"
    char launcher_id[16];    // 예: "LCH-01"
    double x;                // 발사대 X 좌표
    double y;                // 발사대 Y 좌표
    double angle;            // 발사각 (진북 기준)
};

#pragma pack(pop)

#endif // LAUNCH_PROTOCOL_H
