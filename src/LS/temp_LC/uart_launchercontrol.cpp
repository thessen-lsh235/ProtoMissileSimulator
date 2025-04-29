#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstring>
#include <thread>
#include "launchCommand.h"
#include "launcher_config.h" 

#define SERIAL_PORT "/dev/pts/9"
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

// 발사대 상태 수신 스레드
void receiveStatusThread(int fd) {
    while (true) {
        LauncherConfig status{};
        ssize_t len = read(fd, &status, sizeof(status));
        if (len == sizeof(status)) {
            std::cout << "\n[발사대 상태 수신 완료]\n";
            std::cout << "  ID           : " << status.id << "\n";
            std::cout << "  위치         : (" << status.x << ", " << status.y << ")\n";
            std::cout << "  미사일 수    : " << status.missile_count << "\n";
            std::cout << "  미사일 ID    : ";
            for (int id : status.missile_ids) std::cout << id << " ";
            std::cout << "\n";
            std::cout << "  동작 모드    : " << LauncherConfig::modeToString(status.mode) << "\n";
            std::cout << "  고장 상태    : " << (status.is_faulty ? "고장" : "정상") << "\n";
        } else if (len > 0) {
            std::cerr << "[경고] 상태 메시지 일부만 수신됨 (" << len << " bytes)\n";
        } else if (len < 0) {
            perror("UART 상태 수신 실패");
            break;
        }
        usleep(100000);  // 100ms 대기
    }
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

    std::cout << "[임시 발사통제기 실행 - UART 통신 시작]\n";

    // 상태 수신 스레드 실행
    std::thread recv_thread(receiveStatusThread, serial_fd);

    while (true) {
        LaunchCommand cmd{};
        std::cout << "\n[발사명령 입력]\n";
        std::cout << "  발사대 ID : ";
        std::cin >> cmd.launcher_id;
        std::cout << "  미사일 ID : ";
        std::cin >> cmd.missile_id;
        std::cout << "  발사각(도): ";
        std::cin >> cmd.launch_angle;

        ssize_t sent = write(serial_fd, &cmd, sizeof(cmd));
        if (sent > 0) {
            std::cout << "[UART로 발사명령 전송 완료]\n";
        } else {
            perror("UART 전송 실패");
        }

        char cont;
        std::cout << "계속 발사하시겠습니까? (y/n): ";
        std::cin >> cont;
        if (cont == 'n' || cont == 'N') break;
    }

    close(serial_fd);
    recv_thread.join();  // 수신 스레드 종료 대기
    return 0;
}
