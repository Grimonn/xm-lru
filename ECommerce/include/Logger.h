#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <chrono>
#include <format>
#include <filesystem>

enum class LogLevel {
    INFO,
    WARNING,
    ERROR
};

enum class LogCategory {
    CACHE,
    DATABASE,
    SERVICE,
    GENERAL
};

class Logger {
public:
    static void initialize(const std::string& filename);
    static void log(LogLevel level, LogCategory category, const std::string& message);
    static void setLogLevel(LogLevel level);

private:
    static inline std::ofstream logFile;
    static inline LogLevel currentLogLevel = LogLevel::INFO;
    static inline std::mutex logMutex;

    static std::string getCurrentTime();
    static constexpr std::string_view logLevelToString(LogLevel level);
    static constexpr std::string_view logCategoryToString(LogCategory category);
};

#endif
