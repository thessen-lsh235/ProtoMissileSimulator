#include "launcher_config.h"
#include "launcher_config_handler.h"
#include "missileInfo.h"
#include "launchCommand.h"
#include "udp_launcher.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <cstring>

// UART 설정
#define SERIAL_PORT "/dev/pts/8"
#define BAUD_RATE B115200

bool setupSerialPort(int fd) {
    struct termios options;
    if (tcgetattr(fd, &options) < 0) {
        perror("tcgetattr");
        return false;
    }

    cfsetispeed(&options, BAUD_RATE);
    cfsetospeed(&options, BAUD_RATE);

    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    if (tcsetattr(fd, TCSANOW, &options) < 0) {
        perror("tcsetattr");
        return false;
    }
    return true;
}

// LauncherConfig를 UART로 전송하는 함수
void sendLauncherStatusOverSerial(const LauncherConfig& config) {
    int serial_fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY);
    if (serial_fd < 0) {
        perror("Failed to open serial port for status");
        return;
    }

    if (!setupSerialPort(serial_fd)) {
        close(serial_fd);
        return;
    }

    // 간단히 LauncherConfig 구조체 자체를 바이너리로 전송
    ssize_t sent = write(serial_fd, &config, sizeof(config));
    if (sent > 0) {
        std::cout << "[발사대 상태 전송 완료] (" << sent << " bytes)\n";
    } else {
        perror("UART 상태 전송 실패");
    }

    close(serial_fd);
}

void runUartServer() {
    setLauncherStatusHandler(sendLauncherStatusOverSerial);  // ✅ 이벤트 핸들러 등록

    int serial_fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY);
    if (serial_fd < 0) {
        perror("Failed to open serial port");
        return;
    }

    if (!setupSerialPort(serial_fd)) {
        close(serial_fd);
        return;
    }

    std::cout << "[UART Server Start] Listening on " << SERIAL_PORT << "\n";

    constexpr int default_speed = 1000;

    while (true) {
        LaunchCommand cmd{};
        ssize_t read_len = read(serial_fd, &cmd, sizeof(cmd));

        if (read_len == sizeof(cmd)) {
            std::cout << "\n[LaunchCommand 수신 완료]\n";
            std::cout << "  발사대 ID : " << cmd.launcher_id << "\n";
            std::cout << "  미사일 ID : " << cmd.missile_id << "\n";
            std::cout << "  발사각    : " << cmd.launch_angle << " 도\n";

            // 예시: 발사각 변경시 발사대 상태도 업데이트
            extern LauncherConfig g_launcher_config;
            g_launcher_config.x += 10; // (ex) 발사 후 위치 이동했다고 가정
            notifyLauncherStatusChanged();  // ⚡ 속성 변경 알림

            // 원래 missile 발사
            MissileInfo missile;
            missile.missile_id = cmd.missile_id;
            missile.speed = default_speed;
            missile.degree = cmd.launch_angle;
            missile.LS_pos_x = g_launcher_config.x;
            missile.LS_pos_y = g_launcher_config.y;

            sendMissile(missile);
        } else if (read_len > 0) {
            std::cerr << "Partial data received (" << read_len << " bytes)\n";
        } else if (read_len < 0) {
            perror("UART read error");
            break;
        }

        usleep(100000); // 100ms delay
    }

    close(serial_fd);
}
