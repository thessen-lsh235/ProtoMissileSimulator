#include "launcher_config_instance.h"
#include "ini_parser.h"
#include <sstream>
#include <iostream>
#include <cmath>
#include <iomanip>

LauncherConfig g_launcher_config;  

void initLauncherConfig(const std::string& ini_path) {
    IniParser parser;
    auto data = parser.parse(ini_path);
    auto section = parser.getSection("LAUNCHER");

    g_launcher_config.id = std::stoi(section["ID"]);
    g_launcher_config.x = std::stoll(section["X"]);  // long long 직접 파싱
    g_launcher_config.y = std::stoll(section["Y"]);
    g_launcher_config.missile_count = std::stoi(section["MISSILE_COUNT"]);

    std::stringstream ss(section["MISSILE_IDS"]);
    std::string token;
    while (std::getline(ss, token, ',')) {
        token.erase(0, token.find_first_not_of(" \t"));
        token.erase(token.find_last_not_of(" \t") + 1);
        g_launcher_config.missile_ids.push_back(std::stoi(token));
    }

    if (section.find("MODE") != section.end()) {
        g_launcher_config.mode = LauncherConfig::stringToMode(section["MODE"]);
    }

    std::cout << "[LauncherConfig Loaded] \n";
    std::cout << std::fixed << std::setprecision(8);
    std::cout << "  ID       : " << g_launcher_config.id << "\n";
    std::cout << "  X (경도) : " << g_launcher_config.x / 1e8 << "\n";
    std::cout << "  Y (위도) : " << g_launcher_config.y / 1e8 << "\n";
    std::cout << "  MISSILES : " << g_launcher_config.missile_count << "\n";
    std::cout << "  MODE     : " << LauncherConfig::modeToString(g_launcher_config.mode) << "\n";
}