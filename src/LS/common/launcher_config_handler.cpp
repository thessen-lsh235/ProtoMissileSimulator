#include "launcher_config_handler.h"

static LauncherStatusHandler g_statusHandler = nullptr;

void setLauncherStatusHandler(LauncherStatusHandler handler) {
    g_statusHandler = handler;
}

void notifyLauncherStatusChanged() {
    extern LauncherConfig g_launcher_config;  // 발사대 속성 (전역)
    if (g_statusHandler) {
        g_statusHandler(g_launcher_config);   // 현재 전체 상태를 이벤트로 전달
    }
}
