#pragma once
#include <string>

struct Vector3
{
    float x, y, z;
};

class Entity
{
public:
    Entity(const std::string &name, const Vector3 &position, const Vector3 &velocity)
        : name(name), position(position), velocity(velocity) {}

    virtual ~Entity() = default;

    virtual void update() = 0;

    const std::string &getName() const { return name; }
    const Vector3 &getPosition() const { return position; }
    const Vector3 &getVelocity() const { return velocity; }

protected:
    std::string name;
    Vector3 position;
    Vector3 velocity;
};

class SimTarget : public Entity
{
public:
    SimTarget(const std::string &name, const Vector3 &position, const Vector3 &velocity)
        : Entity(name, position, velocity) {}

    void update() override
    {
        // Update target position based on velocity
        position.x += velocity.x;
        position.y += velocity.y;
        position.z += velocity.z;
    }
};

class SimMissile : public Entity
{
public:
    SimMissile(const std::string &name, const Vector3 &position, const Vector3 &velocity)
        : Entity(name, position, velocity) {}

    void update() override
    {
        // Update missile position based on velocity
        position.x += velocity.x;
        position.y += velocity.y;
        position.z += velocity.z;
    }
};
