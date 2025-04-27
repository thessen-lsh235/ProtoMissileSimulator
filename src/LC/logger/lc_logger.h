#pragma once

#include <string>
#include <fstream>
#include <mutex>

class LcLogger {
public:
    LcLogger();
    ~LcLogger();

    bool startLogging(const std::string& filename);
    void logMessage(const std::string& message);
    void stopLogging();

private:
    std::ofstream logFile_;
    std::mutex logMutex_;
};