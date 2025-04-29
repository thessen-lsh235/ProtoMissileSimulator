#pragma once

#include "launcher_config.h"   // 구조체 불러오기

extern LauncherConfig g_launcher_config;  // 발사대 객체 속성 전역변수 
void initLauncherConfig(const std::string& ini_path);  // 초기화 함수
