#include "lc_logger.h"
#include <iostream>
#include <ctime>

LcLogger::LcLogger() {}

LcLogger::~LcLogger() {
    stopLogging();
}

bool LcLogger::startLogging(const std::string& filename) {
    logFile_.open(filename, std::ios::out | std::ios::app);
    if (!logFile_.is_open()) {
        std::cerr << "[LcLogger] 로그 파일 열기 실패" << std::endl;
        return false;
    }
    return true;
}

void LcLogger::logMessage(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex_);

    if (logFile_.is_open()) {
        std::time_t now = std::time(nullptr);
        logFile_ << "[" << std::ctime(&now) << "] " << message << std::endl;
    }
}

void LcLogger::stopLogging() {
    if (logFile_.is_open()) {
        logFile_.close();
    }
}
