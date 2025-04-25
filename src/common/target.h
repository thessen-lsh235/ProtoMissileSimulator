#include "serialize/serializable_base.hpp"
#include "type.h"

#include <cstring>
#include <vector>
#include <memory>
#include <string>

// Target 구조체
struct TargetInfo : public Serializable<TargetInfo>
{
	int id;		   // 모의 표적 식별 id
	double pos_x;  // 모의 표적 x 좌표
	double pos_y;  // 모의 표적 y 좌표
	int speed;	   // 모의 표적 속도
	double degree; // 모의 표적 이동각

	std::vector<uint8_t> serializeImpl() const
	{
		std::vector<uint8_t> buffer(1 + sizeof(id) + sizeof(pos_x) + sizeof(pos_y) + sizeof(speed) + sizeof(degree));
		buffer[0] = static_cast<uint8_t>(DataType::Target); // TargetInfo 타입 식별자
		size_t offset = 1;
		std::memcpy(buffer.data() + offset, &id, sizeof(id));
		offset += sizeof(id);
		std::memcpy(buffer.data() + offset, &pos_x, sizeof(pos_x));
		offset += sizeof(pos_x);
		std::memcpy(buffer.data() + offset, &pos_y, sizeof(pos_y));
		offset += sizeof(pos_y);
		std::memcpy(buffer.data() + offset, &speed, sizeof(speed));
		offset += sizeof(speed);
		std::memcpy(buffer.data() + offset, &degree, sizeof(degree));
		return buffer;
	}

	void deserializeImpl(const std::vector<uint8_t> &buffer)
	{
		size_t offset = 0;
		std::memcpy(&id, buffer.data() + offset, sizeof(id));
		offset += sizeof(id);
		std::memcpy(&pos_x, buffer.data() + offset, sizeof(pos_x));
		offset += sizeof(pos_x);
		std::memcpy(&pos_y, buffer.data() + offset, sizeof(pos_y));
		offset += sizeof(pos_y);
		std::memcpy(&speed, buffer.data() + offset, sizeof(speed));
		offset += sizeof(speed);
		std::memcpy(&degree, buffer.data() + offset, sizeof(degree));
	}
};