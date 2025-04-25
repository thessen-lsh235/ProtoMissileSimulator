#pragma once
#include <cstdint>

// 타입 식별자 정의
enum class DataType : uint8_t
{
	Missile = 0x01,
	Target = 0x02
};