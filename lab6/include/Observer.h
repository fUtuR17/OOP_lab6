#pragma once
#include <string>
#include <fstream>
#include <iostream>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const std::string& message) = 0;
};

class ConsoleObserver : public Observer {
public:
    void update(const std::string& message) override;
};

class FileObserver : public Observer {
private:
    std::ofstream logFile;
public:
    explicit FileObserver(const std::string& filename);
    ~FileObserver();
    void update(const std::string& message) override;
};