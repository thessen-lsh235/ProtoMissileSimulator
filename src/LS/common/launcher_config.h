#pragma once
#include <vector>
#include <string>

struct LauncherConfig {
    int id = 0;
    double x = 0.0;
    double y = 0.0;
    int missile_count = 0;
    std::vector<int> missile_ids;
};

extern LauncherConfig g_launcher_config; // 전역으로 선언
void initLauncherConfig(const std::string& ini_path); // 초기화 함수 선언
