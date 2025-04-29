#include "launcher_config.h"
#include "udp_launcher.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <cstring>

#define SERIAL_PORT "/dev/pts/12"
#define BAUD_RATE B115200

#pragma pack(push, 1)
struct LaunchCommand {
    int launcher_id;
    int missile_id;
    double launch_angle;
};
#pragma pack(pop)

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

void runUartServer() {
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
