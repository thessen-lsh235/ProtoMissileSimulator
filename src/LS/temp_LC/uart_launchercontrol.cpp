#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstring>
#include <thread>
#include "launcher_config.h"
#include "launcher_message.h"

#define SERIAL_PORT "/dev/pts/7"
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
        LauncherStatusMessage msg{};
        ssize_t len = read(fd, &msg, sizeof(msg));

        if (len == sizeof(msg)) {
            std::cout << "\n[발사대 상태 수신 완료]\n";
            std::cout << "  ID           : " << msg.id << "\n";
            std::cout << "  위치         : (" << msg.x << ", " << msg.y << ")\n";
            std::cout << "  미사일 수    : " << msg.missile_count << "\n";
            std::cout << "  미사일 ID    : ";
            for (int i = 0; i < msg.missile_count; ++i) {
                std::cout << msg.missile_ids[i] << " ";
            }
            std::cout << "\n";
            std::cout << "  동작 모드    : " << LauncherConfig::modeToString(msg.mode) << "\n";
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

    std::thread recv_thread(receiveStatusThread, serial_fd);

    while (true) {
        LauncherMessage msg{};
        int command;

        while (true) {
            std::cout << "\n[명령 선택] 1: 발사, 2: 이동, 3: 모드 변경, 4: 상태 요청, 0: 종료 → ";
            if (std::cin >> command && command >= 0 && command <= 4) break;

            std::cerr << "[오류] 0~4 사이의 숫자를 입력하세요.\n";
            std::cin.clear();
            std::cin.ignore(1000, '\n');
        }

        if (command == 0) break;

        switch (command) {
            case 1:
                msg.type = CommandType::LAUNCH;
                while (true) {
                    std::cout << "  발사대 ID: ";
                    if (std::cin >> msg.launch.launcher_id) break;
                    std::cerr << "[오류] 숫자를 입력하세요.\n";
                    std::cin.clear(); std::cin.ignore(1000, '\n');
                }
                while (true) {
                    std::cout << "  미사일 ID: ";
                    if (std::cin >> msg.launch.missile_id) break;
                    std::cerr << "[오류] 숫자를 입력하세요.\n";
                    std::cin.clear(); std::cin.ignore(1000, '\n');
                }
                while (true) {
                    std::cout << "  발사각(도): ";
                    if (std::cin >> msg.launch.launch_angle) break;
                    std::cerr << "[오류] 숫자를 입력하세요.\n";
                    std::cin.clear(); std::cin.ignore(1000, '\n');
                }
                break;

            case 2:
                msg.type = CommandType::MOVE;
                while (true) {
                    std::cout << "  이동 X: ";
                    if (std::cin >> msg.move.new_x) break;
                    std::cerr << "[오류] 숫자를 입력하세요.\n";
                    std::cin.clear(); std::cin.ignore(1000, '\n');
                }
                while (true) {
                    std::cout << "  이동 Y: ";
                    if (std::cin >> msg.move.new_y) break;
                    std::cerr << "[오류] 숫자를 입력하세요.\n";
                    std::cin.clear(); std::cin.ignore(1000, '\n');
                }
                break;

            case 3:
                msg.type = CommandType::MODE_CHANGE;
                while (true) {
                    std::cout << "  모드 (0: ENGAGEMENT, 1: MOVEMENT): ";
                    int mode_input;
                    if (std::cin >> mode_input && (mode_input == 0 || mode_input == 1)) {
                        msg.mode_change.new_mode = static_cast<OperationMode>(mode_input);
                        break;
                    }
                    std::cerr << "[오류] 0 또는 1만 입력하세요.\n";
                    std::cin.clear(); std::cin.ignore(1000, '\n');
                }
                break;

            case 4:
                msg.type = CommandType::STATUS_REQUEST;
                msg.status_request.dummy = 0;
                break;
        }

        ssize_t sent = write(serial_fd, &msg, sizeof(msg));
        if (sent > 0) {
            std::cout << "[UART로 명령 전송 완료]\n";
        } else {
            perror("UART 전송 실패");
        }
    }

    close(serial_fd);
    recv_thread.join();
    return 0;
}