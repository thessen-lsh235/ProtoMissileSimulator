#include "launcher_config.h"
#include "launcher_config_handler.h"
#include "udp_launcher.h"
#include "launcher_message.h" 
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <cstring>
#include <cmath>
#include <thread>
#include <atomic>
#include <iomanip>
#define SERIAL_PORT "/dev/pts/8"
#define BAUD_RATE B115200

extern LauncherConfig g_launcher_config;

std::atomic<bool> movement_interrupted{false};
std::thread movement_thread;

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

    write(serial_fd, &msg, sizeof(msg));
    close(serial_fd);
}

void moveLauncherToTarget(long long target_x, long long target_y) {
    constexpr double speed_kmph = 50.0;
    constexpr double update_interval_sec = 0.1;

    auto& config = g_launcher_config;
    config.mode = OperationMode::MOVEMENT;
    notifyLauncherStatusChanged();

    while (!movement_interrupted.load()) {
        long long dx = target_x - config.x;
        long long dy = target_y - config.y;

        // 도달 기준: x, y 각각 100단위 (1e-6도 ≈ 1cm 정도)
        if (std::abs(dx) < 100 && std::abs(dy) < 100) {
            std::cout << "[이동 완료] 목표 좌표에 도달했습니다.\n";
            break;
        }

        // 방향 계산
        double distance = std::sqrt(static_cast<double>(dx) * dx + static_cast<double>(dy) * dy);
        double dir_x = dx / distance;
        double dir_y = dy / distance;

        // 이동 거리 계산 (m/s → m/tick → 스케일 적용)
        double dist_per_tick = (speed_kmph * 1000.0 / 3600.0) * update_interval_sec;
        long long delta_x = static_cast<long long>(dir_x * dist_per_tick * 1e8);
        long long delta_y = static_cast<long long>(dir_y * dist_per_tick * 1e8);

        // 너무 작아서 0이 되는 경우 보정
        if (delta_x == 0) delta_x = (dx > 0 ? 1 : -1);
        if (delta_y == 0) delta_y = (dy > 0 ? 1 : -1);

        long long new_x = config.x + delta_x;
        long long new_y = config.y + delta_y;
        
        if ((dx > 0 && new_x > target_x) || (dx < 0 && new_x < target_x)) {
            config.x = target_x;
        } else {
            config.x = new_x;
        }
        
        if ((dy > 0 && new_y > target_y) || (dy < 0 && new_y < target_y)) {
            config.y = target_y;
        } else {
            config.y = new_y;
        }
        
        // 실시간 위치 출력
        std::cout << std::fixed << std::setprecision(8);
        std::cout << "[이동 중] 현재 위치 → 위도(y): " << config.y / 1e8
                  << ", 경도(x): " << config.x / 1e8 << "\n";

        notifyLauncherStatusChanged();
        usleep(static_cast<int>(update_interval_sec * 1e6));
    }

    // 도중에 끊기지 않았을 경우 STOP 모드로 변경
    if (!movement_interrupted.load()) {
        config.mode = OperationMode::STOP;
        notifyLauncherStatusChanged();
    }

    movement_interrupted.store(false);  // 상태 초기화
}


void runUartServer() {
    setLauncherStatusHandler(sendLauncherStatusOverSerial);

    int serial_fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY);
    if (serial_fd < 0 || !setupSerialPort(serial_fd)) {
        perror("Failed to open or setup serial port");
        return;
    }

    std::cout << "[UART Server Start] Listening on " << SERIAL_PORT << "\n";
    constexpr int default_speed = 1000;

    while (true) {
        LauncherMessage msg{};
        ssize_t read_len = read(serial_fd, &msg, sizeof(msg));

        if (read_len == sizeof(msg)) {
            switch (msg.type) {
                case CommandType::LAUNCH:
                case CommandType::MODE_CHANGE:
                    if (g_launcher_config.mode == OperationMode::MOVEMENT) {
                        std::cerr << "[오류] 이동 중에는 발사 또는 모드 변경이 불가능합니다.\n";
                        break;
                    }
                    if (msg.type == CommandType::LAUNCH) {
                        std::cout << "\n[발사 명령 수신]\n";
                        std::cout << "  미사일 ID: " << msg.launch.missile_id << " / 각도: " << msg.launch.launch_angle << "\n";

                        MissileInfo missile;
                        missile.missile_id = msg.launch.missile_id;
                        missile.speed = default_speed;
                        missile.degree = msg.launch.launch_angle;
                        missile.LS_pos_x = g_launcher_config.x;
                        missile.LS_pos_y = g_launcher_config.y;
                        sendMissile(missile);
                    } else {
                        std::cout << "\n[모드 변경 수신]\n";
                        g_launcher_config.mode = msg.mode_change.new_mode;
                        notifyLauncherStatusChanged();
                    }
                    break;

                case CommandType::MOVE:
                    std::cout << "\n[이동 명령 수신]\n";
                    movement_interrupted.store(true);
                    if (movement_thread.joinable()) movement_thread.join();
                    movement_interrupted.store(false);
                    movement_thread = std::thread(moveLauncherToTarget, msg.move.new_x, msg.move.new_y);
                    break;

                case CommandType::STATUS_REQUEST:
                    std::cout << "\n[상태 요청 수신] 현재 상태 전송 중...\n";
                    notifyLauncherStatusChanged();
                    break;

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

        usleep(100000);
    }

    close(serial_fd);
    if (movement_thread.joinable()) movement_thread.join();
}
