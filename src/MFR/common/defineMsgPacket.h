#ifndef DEFINE_MSG_PACKET_H
#define DEFINE_MSG_PACKET_H

#include <cstdint>

#pragma pack(push, 1)

struct TargetInfo 
{
    uint32_t targetId;
    float latitude;
    float longitude;
    float altitude;
};

struct MissileInfo 
{
    uint32_t missileId;
    float speed;
    float heading;
    float distanceToTarget;
    float latitude;
    float longitude;
    float altitude;
};

struct Message 
{
    TargetInfo target;
    MissileInfo missile;
};

#pragma pack(pop)

#endif