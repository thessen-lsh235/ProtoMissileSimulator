#include "launcher_config.h"
#include "ini_parser.h" 
#include <sstream>
#include <iostream>

LauncherConfig g_launcher_config;

void initLauncherConfig(const std::string& ini_path) {
    IniParser parser;
    auto data = parser.parse(ini_path);
    auto section = parser.getSection("LAUNCHER");

    g_launcher_config.id = std::stoi(section["ID"]);
    g_launcher_config.x = std::stod(section["X"]);
    g_launcher_config.y = std::stod(section["Y"]);
    g_launcher_config.missile_count = std::stoi(section["MISSILE_COUNT"]);

    std::stringstream ss(section["MISSILE_IDS"]);
    std::string token;
    while (std::getline(ss, token, ',')) {
        token.erase(0, token.find_first_not_of(" \t"));
        token.erase(token.find_last_not_of(" \t") + 1);
        g_launcher_config.missile_ids.push_back(std::stoi(token));
    }

    std::cout << "[LauncherConfig Loaded] ID: " << g_launcher_config.id
              << ", X: " << g_launcher_config.x
              << ", Y: " << g_launcher_config.y
              << ", MISSILES: " << g_launcher_config.missile_count << "\n";
}
