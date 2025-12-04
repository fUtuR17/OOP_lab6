#include "Observer.h"

void ConsoleObserver::update(const std::string& message) {
    std::cout << "[BATTLE] " << message << std::endl;
}

FileObserver::FileObserver(const std::string& filename) {
    logFile.open(filename, std::ios::app);
    if (!logFile.is_open()) {
        throw std::runtime_error("Cannot open log file: " + filename);
    }
}

FileObserver::~FileObserver() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void FileObserver::update(const std::string& message) {
    if (logFile.is_open()) {
        logFile << message << std::endl;
    }
}