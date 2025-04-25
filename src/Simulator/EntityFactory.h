#pragma once
#include "Entities.h"
#include <memory>

class EntityFactory
{
public:
	static std::unique_ptr<Entity> createTarget(const std::string &name, const Vector3 &position, const Vector3 &velocity)
	{
		return std::make_unique<SimTarget>(name, position, velocity);
	}

	static std::unique_ptr<Entity> createMissile(const std::string &name, const Vector3 &position, const Vector3 &velocity)
	{
		return std::make_unique<SimMissile>(name, position, velocity);
	}
};
