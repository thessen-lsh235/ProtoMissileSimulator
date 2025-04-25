#include "serialize/serializable_base.hpp"

#include <cstring>
#include <vector>
#include <memory>
#include <string>

struct MissileInfo : public Serializable<MissileInfo>
{
	double LS_pos_x; // 발사대 x 좌표
	double LS_pos_y; // 발사대 y 좌표
	int speed;		 // 미사일 속도
	double degree;	 // 발사각

	std::vector<uint8_t> serializeImpl() const
	{
		std::vector<uint8_t> buffer(sizeof(LS_pos_x) + sizeof(LS_pos_y) + sizeof(speed) + sizeof(degree));
		size_t offset = 0;
		std::memcpy(buffer.data() + offset, &LS_pos_x, sizeof(LS_pos_x));
		offset += sizeof(LS_pos_x);
		std::memcpy(buffer.data() + offset, &LS_pos_y, sizeof(LS_pos_y));
		offset += sizeof(LS_pos_y);
		std::memcpy(buffer.data() + offset, &speed, sizeof(speed));
		offset += sizeof(speed);
		std::memcpy(buffer.data() + offset, &degree, sizeof(degree));
		return buffer;
	}

	void deserializeImpl(const std::vector<uint8_t> &buffer)
	{
		size_t offset = 0;
		std::memcpy(&LS_pos_x, buffer.data() + offset, sizeof(LS_pos_x));
		offset += sizeof(LS_pos_x);
		std::memcpy(&LS_pos_y, buffer.data() + offset, sizeof(LS_pos_y));
		offset += sizeof(LS_pos_y);
		std::memcpy(&speed, buffer.data() + offset, sizeof(speed));
		offset += sizeof(speed);
		std::memcpy(&degree, buffer.data() + offset, sizeof(degree));
	}
};