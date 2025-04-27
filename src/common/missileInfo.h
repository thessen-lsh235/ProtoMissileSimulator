#include "serialize/serializable_base.hpp"
#include "type.h"

#include <cstring>
#include <memory>
#include <string>
#include <vector>

struct MissileInfo : public Serializable<MissileInfo> {
    int missile_id;  // 미사일 ID
    double LS_pos_x; // 발사대 x 좌표
    double LS_pos_y; // 발사대 y 좌표
    int speed;       // 미사일 속도
    double degree;   // 발사각

    std::vector<uint8_t> serializeImpl() const {
        std::vector<uint8_t> buffer(1 + sizeof(MissileInfo));
        buffer[0] = static_cast<uint8_t>(DataType::Missile); // MissileInfo 타입 식별자
        std::memcpy(buffer.data() + 1, this, sizeof(MissileInfo));
        return buffer;
    }

    void deserializeImpl(const std::vector<uint8_t> &buffer) {
        std::memcpy(this, buffer.data() + 1, sizeof(MissileInfo)); // 타입 식별자 건너뜀
    }
};