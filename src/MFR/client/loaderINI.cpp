#include "loaderINI.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

void loadMessageFromIni(const std::string& filename, Message& msg) 
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[에러] ini 파일을 열 수 없습니다: " << filename << std::endl;
        return;
    }

    std::string line, currentSection;
    std::unordered_map<std::string, std::string> kv;

    while (std::getline(file, line)) {
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        if (line.empty() || line[0] == ';' || line[0] == '#') continue;

        if (line.front() == '[' && line.back() == ']') {
            currentSection = line.substr(1, line.length() - 2);
            continue;
        }

        size_t eqPos = line.find('=');
        if (eqPos != std::string::npos) {
            std::string key = line.substr(0, eqPos);
            std::string value = line.substr(eqPos + 1);
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            kv[currentSection + "." + key] = value;
        }
    }

    msg.target.targetId = std::stoi(kv["Target.targetId"]);
    msg.target.latitude = std::stof(kv["Target.latitude"]);
    msg.target.longitude = std::stof(kv["Target.longitude"]);
    msg.target.altitude = std::stof(kv["Target.altitude"]);

    msg.missile.missileId = std::stoi(kv["Missile.missileId"]);
    msg.missile.speed = std::stof(kv["Missile.speed"]);
    msg.missile.heading = std::stof(kv["Missile.heading"]);
    msg.missile.distanceToTarget = std::stof(kv["Missile.distanceToTarget"]);
    msg.missile.latitude = std::stof(kv["Missile.latitude"]);
    msg.missile.longitude = std::stof(kv["Missile.longitude"]);
    msg.missile.altitude = std::stof(kv["Missile.altitude"]);
}