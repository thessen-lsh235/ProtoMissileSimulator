#include "Entities.h"
#include <fmt/format.h>
#include <math.h>

class Missile : public Entity {
  private:
    int id;
    double x, y;
    double speed;  // 속도 (단위: 거리/초)
    double degree; // 각도 (도 단위)

  public:
    Missile(int id, double x, double y, double speed, double degree) : id(id), x(x), y(y), speed(speed), degree(degree) {}

    void updatePosition(double delta_time) override {
        if (speed > 0) {                           // 속도가 0보다 클 때만 이동
            double radian = degree * M_PI / 180.0; // 각도를 라디안으로 변환
            x += speed * delta_time * cos(radian); // x 좌표 업데이트
            y += speed * delta_time * sin(radian); // y 좌표 업데이트
        }

        // 맵 경계 초과 여부 확인
        checkBounds(std::to_string(id), x, y);

        fmt::print("[{}] Missile updated - x: {:.2f}, y: {:.2f}, degree: {:.2f}, speed: {:.2f}\n", id, x, y, degree, speed);
    }

    double getX() const { return x; }
    double getY() const { return y; }
    int getId() const { return id; }
};