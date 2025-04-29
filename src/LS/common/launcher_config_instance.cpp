#include "launcher_config_instance.h"
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

    if (section.find("MODE") != section.end()) {
        g_launcher_config.mode = LauncherConfig::stringToMode(section["MODE"]);
    }

    if (section.find("FAULT") != section.end()) {
        g_launcher_config.is_faulty = (section["FAULT"] == "1");
    }
    std::cout << "[LauncherConfig Loaded] ID: " << g_launcher_config.id
              << "\n X: " << g_launcher_config.x
              << "\n Y: " << g_launcher_config.y
              << "\n MISSILES: " << g_launcher_config.missile_count
              << "\n MODE: " << (g_launcher_config.mode == OperationMode::ENGAGEMENT ? "ENGAGEMENT" : "MOVEMENT")
              << "\n FAULTY: " << (g_launcher_config.is_faulty ? "YES" : "NO")
              << "\n";
}
