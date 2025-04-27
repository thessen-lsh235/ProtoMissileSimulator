#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstring>
#include "missile.h"
#include "ini.h" // inih 라이브러리 추가

#define SERIAL_PORT "/dev/pts/8"
#define BAUD_RATE B115200
#define INI_FILE_PATH "../launcher_config.ini"

#pragma pack(push, 1)
struct LaunchCommand {
    int launcher_id;
    int missile_id;
    double launch_angle;
};
#pragma pack(pop)

extern void sendMissile(const MissileInfo& missile);

// UART 설정 함수
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

// launcher_config.ini 읽기 함수
struct LauncherConfig {
    double x;
    double y;
};

int iniHandler(void* user, const char* section, const char* name, const char* value) {
    LauncherConfig* config = (LauncherConfig*)user;

    if (strcmp(section, "LAUNCHER") == 0) {
        if (strcmp(name, "X") == 0) {
            config->x = atof(value);
        } else if (strcmp(name, "Y") == 0) {
            config->y = atof(value);
        }
    }
    return 1;
}

bool readLauncherPosition(double& x, double& y) {
    LauncherConfig config{};

    if (ini_parse(INI_FILE_PATH, iniHandler, &config) < 0) {
        std::cerr << "Cannot load launcher_config.ini\n";
        return false;
    }

    x = config.x;
    y = config.y;
    return true;
}

int main() {
    int serial_fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY);
    if (serial_fd < 0) {
        perror("Failed to open serial port");
        return 1;
    }

    if (!setupSerialPort(serial_fd)) {
        close(serial_fd);
        return 1;
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

            double launcher_x = 0, launcher_y = 0;
            if (readLauncherPosition(launcher_x, launcher_y)) {
                missile.LS_pos_x = launcher_x;
                missile.LS_pos_y = launcher_y;

                std::cout << "[Launcher Config Loaded] X: " << launcher_x << ", Y: " << launcher_y << "\n";
                sendMissile(missile);
            } else {
                std::cerr << "Failed to load launcher position.\n";
            }
        } else if (read_len > 0) {
            std::cerr << "Partial data received (" << read_len << " bytes)\n";
        } else if (read_len < 0) {
            perror("UART read error");
            break;
        }

        usleep(100000); // 100ms delay
    }

    close(serial_fd);
    return 0;
}
