#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstring>

// UART 설정
#define SERIAL_PORT "/dev/pts/7"
#define BAUD_RATE B115200

#pragma pack(push, 1)
struct LaunchCommand {
    int launcher_id;       // 발사대 ID
    int missile_id;        // 미사일 ID
    double launch_angle;   // 발사각
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

int main() {
    int serial_fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);
    if (serial_fd < 0) {
        perror("Failed to open serial port");
        return 1;
    }

    if (!setupSerialPort(serial_fd)) {
        close(serial_fd);
        return 1;
    }

    std::cout << "[임시 발사통제기 실행]\n";

    while (true) {
        LaunchCommand cmd{};
        std::cout << "발사대 ID 입력: ";
        std::cin >> cmd.launcher_id;
        std::cout << "미사일 ID 입력: ";
        std::cin >> cmd.missile_id;
        std::cout << "발사각 입력 (도 단위): ";
        std::cin >> cmd.launch_angle;

        ssize_t sent = write(serial_fd, &cmd, sizeof(cmd));
        if (sent > 0) {
            std::cout << "[UART로 발사명령 전송 완료]\n";
        } else {
            perror("UART 전송 실패");
        }

        char cont;
        std::cout << "계속 보내시겠습니까? (y/n): ";
        std::cin >> cont;
        if (cont == 'n' || cont == 'N') break;
    }

    close(serial_fd);
    return 0;
}
