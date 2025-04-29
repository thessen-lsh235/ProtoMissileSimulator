#pragma once
#include "launcher_config.h"
#include <functional>

// 이벤트 핸들러 타입 (LauncherConfig 전체를 보냄)
using LauncherStatusHandler = std::function<void(const LauncherConfig&)>;

// 핸들러 등록
void setLauncherStatusHandler(LauncherStatusHandler handler);

// 속성 변경 알림
void notifyLauncherStatusChanged();
