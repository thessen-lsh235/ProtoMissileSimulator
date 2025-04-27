#include "lc_serial.h"
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstring>
#include <iostream>

LcSerial::LcSerial(const std::string& device, int baudrate)
    : device_(device), baudrate_(baudrate), fd_(-1)
{
}

LcSerial::~LcSerial()
{
    closeSerial();
}

bool LcSerial::openSerial()
{
    fd_ = open(device_.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (fd_ < 0) {
        std::cerr << "[Serial] 포트 열기 실패" << std::endl;
        return false;
    }

    struct termios tty{};
    if (tcgetattr(fd_, &tty) != 0) {
        std::cerr << "[Serial] 속성 가져오기 실패" << std::endl;
        close(fd_);
        return false;
    }

    cfsetospeed(&tty, B9600);
    cfsetispeed(&tty, B9600);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit chars
    tty.c_iflag &= ~IGNBRK;                     // ignore break signal
    tty.c_lflag = 0;                            // no signaling chars, no echo
    tty.c_oflag = 0;                            // no remapping, no delays
    tty.c_cc[VMIN] = 1;                         // read doesn't block
    tty.c_cc[VTIME] = 1;                        // 0.1 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);      // shut off xon/xoff ctrl
    tty.c_cflag |= (CLOCAL | CREAD);             // ignore modem controls
    tty.c_cflag &= ~(PARENB | PARODD);           // no parity
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr(fd_, TCSANOW, &tty) != 0) {
        std::cerr << "[Serial] 속성 설정 실패" << std::endl;
        close(fd_);
        return false;
    }

    return true;
}

bool LcSerial::sendMessage(const std::string& message)
{
    if (fd_ < 0) return false;
    return write(fd_, message.c_str(), message.size()) >= 0;
}

std::string LcSerial::receiveMessage()
{
    if (fd_ < 0) return "";

    char buffer[1024] = {0};
    int len = read(fd_, buffer, sizeof(buffer));
    if (len > 0) {
        return std::string(buffer, len);
    }
    return "";
}

void LcSerial::closeSerial()
{
    if (fd_ >= 0) {
        close(fd_);
        fd_ = -1;
    }
}
