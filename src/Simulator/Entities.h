#pragma once
#include <fmt/format.h>
#include <stdexcept>
#include <string>

class Entity {
  protected:
    static constexpr double MAP_SIZE_X = 5000.0; // 맵의 최대 x 크기
    static constexpr double MAP_SIZE_Y = 5000.0; // 맵의 최대 y 크기

    // 맵 경계 초과 여부 확인
    void checkBounds(const std::string &name, double x, double y) const {
        if (x < 0 || x > MAP_SIZE_X || y < 0 || y > MAP_SIZE_Y) {
            fmt::print("[{}] Entity out of bounds and removed - x: {:.2f}, y: {:.2f}\n", name, x, y);
            throw std::runtime_error(fmt::format("[{}] Entity removed due to out-of-bounds", name));
        }
    }

  public:
    virtual void updatePosition(double delta_time) = 0;
    virtual ~Entity() = default;
};
