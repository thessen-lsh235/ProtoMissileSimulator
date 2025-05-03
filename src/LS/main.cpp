#include "launcher_config_instance.cpp"
#include "uart_server.h"

int main() {
    initLauncherConfig("../common/launcher_config.ini");
    runUartServer(); // UART 서버 실행
    return 0;
}
