#include "launcher_config.h"
#include "launcher_config_handler.h"
#include "udp_launcher.h"
#include "launcher_message.h" 
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <cstring>

#define SERIAL_PORT "/dev/pts/8"
#define BAUD_RATE B115200

extern LauncherConfig g_launcher_config;

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

// 상태를 직렬 포트로 전송
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

    LauncherStatusMessage msg{};
    msg.id = config.id;
    msg.x = config.x;
    msg.y = config.y;
    msg.missile_count = config.missile_count;
    for (int i = 0; i < config.missile_count && i < 10; ++i) {
        msg.missile_ids[i] = config.missile_ids[i];
    }
    msg.mode = config.mode;

    ssize_t sent = write(serial_fd, &msg, sizeof(msg));
    if (sent > 0) {
        std::cout << "[발사대 상태 전송 완료] (" << sent << " bytes)\n";
    } else {
        perror("UART 상태 전송 실패");
    }

    close(serial_fd);
}

void runUartServer() {
    setLauncherStatusHandler(sendLauncherStatusOverSerial);

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
        LauncherMessage msg{};
        ssize_t read_len = read(serial_fd, &msg, sizeof(msg));

        if (read_len == sizeof(msg)) {
            switch (msg.type) {
                case CommandType::LAUNCH: {
                    std::cout << "\n[발사 명령 수신]\n";
                    std::cout << "  미사일 ID: " << msg.launch.missile_id
                              << " / 각도: " << msg.launch.launch_angle << "\n";

                    MissileInfo missile;
                    missile.missile_id = msg.launch.missile_id;
                    missile.speed = default_speed;
                    missile.degree = msg.launch.launch_angle;
                    missile.LS_pos_x = g_launcher_config.x;
                    missile.LS_pos_y = g_launcher_config.y;

                    sendMissile(missile);
                    break;
                }

                case CommandType::MOVE: {
                    std::cout << "\n[이동 명령 수신]\n";
                    g_launcher_config.x = msg.move.new_x;
                    g_launcher_config.y = msg.move.new_y;
                    std::cout << "  새로운 위치: (" << msg.move.new_x << ", " << msg.move.new_y << ")\n";
                    notifyLauncherStatusChanged();  // 상태 전송
                    break;
                }

                case CommandType::MODE_CHANGE: {
                    std::cout << "\n[모드 변경 수신]\n";
                    g_launcher_config.mode = msg.mode_change.new_mode;
                    std::cout << "  새로운 모드: " << LauncherConfig::modeToString(g_launcher_config.mode) << "\n";
                    notifyLauncherStatusChanged();  // 상태 전송
                    break;
                }

                case CommandType::STATUS_REQUEST: {
                    std::cout << "\n[상태 요청 수신] 현재 상태 전송 중...\n";
                    notifyLauncherStatusChanged();  // 상태 전송
                    break;
                }

                default:
                    std::cerr << "[오류] 알 수 없는 명령 타입입니다.\n";
                    break;
            }
        } else if (read_len > 0) {
            std::cerr << "[경고] 메시지 일부만 수신됨 (" << read_len << " bytes)\n";
        } else if (read_len < 0) {
            perror("UART read error");
            break;
        }

        usleep(100000); // 100ms
    }

    close(serial_fd);
}
