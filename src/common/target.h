#include "serialize/serializable_base.hpp"
#include "type.h"

#include <cstring>
#include <memory>
#include <string>
#include <vector>

#pragma pack(push, 1) // 1바이트 단위로 정렬
struct TargetInfo : public Serializable<TargetInfo> {
    int id;        // 표적 ID
    double pos_x;  // x 좌표
    double pos_y;  // y 좌표
    int speed;     // 속도
    double degree; // 이동 각도

    std::vector<uint8_t> serializeImpl() const {
        std::vector<uint8_t> buffer(1 + sizeof(TargetInfo));
        buffer[0] = static_cast<uint8_t>(DataType::Target); // 타입 식별자 추가
        std::memcpy(buffer.data() + 1, this, sizeof(TargetInfo));
        return buffer;
    }

    void deserializeImpl(const std::vector<uint8_t> &buffer) {
        std::memcpy(this, buffer.data() + 1, sizeof(TargetInfo)); // 타입 식별자 건너뜀
    }
};
#pragma pack(pop) // 정렬 설정 복원