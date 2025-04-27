#pragma once

#include <string>

class LcSerial
{
public:
    LcSerial(const std::string& device, int baudrate);
    ~LcSerial();

    bool openSerial();
    bool sendMessage(const std::string& message);
    std::string receiveMessage();
    void closeSerial();

private:
    std::string device_;
    int baudrate_;
    int fd_; // file descriptor for serial port
};
