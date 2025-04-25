#ifndef COMMON_H
#define COMMON_H

#include "util.h" // get_val 함수 사용을 위해 포함
#include <string>
#include <sstream>

// 명령 유형 정의
enum class CommandType {
    MISSILE,
    TARGET,
    UNKNOWN
};

// Target 구조체
struct Target {
    std::string name;
    float pos[3]; // x, y, z
    float vel[3]; // vx, vy, vz

    // Target을 문자열로 직렬화
    std::string toString() const {
        std::ostringstream oss;
        oss << "{"
            << "\"type\":\"target\","
            << "\"name\":\"" << name << "\","
            << "\"pos\":[" << pos[0] << "," << pos[1] << "," << pos[2] << "],"
            << "\"vel\":[" << vel[0] << "," << vel[1] << "," << vel[2] << "]"
            << "}";
        return oss.str();
    }

    // 문자열을 Target으로 역직렬화
    static Target fromString(const std::string& msg) {
        Target target;
        target.name = get_val(msg, "name");
        target.pos[0] = std::stof(get_val(msg, "pos"));
        target.pos[1] = std::stof(msg.substr(msg.find(",", msg.find("pos")) + 1));
        target.pos[2] = std::stof(msg.substr(msg.find(",", msg.find(",", msg.find("pos")) + 1) + 1));
        target.vel[0] = std::stof(get_val(msg, "vel"));
        target.vel[1] = std::stof(msg.substr(msg.find(",", msg.find("vel")) + 1));
        target.vel[2] = std::stof(msg.substr(msg.find(",", msg.find(",", msg.find("vel")) + 1) + 1));
        return target;
    }
};

// Missile 구조체
struct Missile {
    std::string name;
    float pos[3]; // x, y, z
    float vel[3]; // vx, vy, vz

    // Missile을 문자열로 직렬화
    std::string toString() const {
        std::ostringstream oss;
        oss << "{"
            << "\"type\":\"missile\","
            << "\"name\":\"" << name << "\","
            << "\"pos\":[" << pos[0] << "," << pos[1] << "," << pos[2] << "],"
            << "\"vel\":[" << vel[0] << "," << vel[1] << "," << vel[2] << "]"
            << "}";
        return oss.str();
    }

    // 문자열을 Missile으로 역직렬화
    static Missile fromString(const std::string& msg) {
        Missile missile;
        missile.name = get_val(msg, "name");
        missile.pos[0] = std::stof(get_val(msg, "pos"));
        missile.pos[1] = std::stof(msg.substr(msg.find(",", msg.find("pos")) + 1));
        missile.pos[2] = std::stof(msg.substr(msg.find(",", msg.find(",", msg.find("pos")) + 1) + 1));
        missile.vel[0] = std::stof(get_val(msg, "vel"));
        missile.vel[1] = std::stof(msg.substr(msg.find(",", msg.find("vel")) + 1));
        missile.vel[2] = std::stof(msg.substr(msg.find(",", msg.find(",", msg.find("vel")) + 1) + 1));
        return missile;
    }
};

#endif // COMMON_H
