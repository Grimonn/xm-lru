#include "Logger.h"
#include <iomanip>
#include <filesystem>


void Logger::initialize(const std::string& filename) {
    namespace fs = std::filesystem;
    std::scoped_lock lock(logMutex);

    try {
        // Ensure the directory exists
        fs::path logPath(filename);
        if (logPath.has_parent_path()) {
            fs::create_directories(logPath.parent_path());
        }

        logFile.open(filename, std::ios::app);
        if (!logFile.is_open()) {
            throw std::ios_base::failure("Failed to open log file.");
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Logger initialization failed: " << e.what() << std::endl;
    }
}

void Logger::log(LogLevel level, LogCategory category, const std::string& message) {
    if (level < currentLogLevel) return;

    std::scoped_lock lock(logMutex);
    auto logMessage = std::format("{} [{}] [{}] {}",
        getCurrentTime(),
        logLevelToString(level),
        logCategoryToString(category),
        message);

    if (logFile.is_open()) {
        logFile << logMessage << std::endl;
    }
    else {
        std::cerr << "Log file not open. Logging to console: " << logMessage << std::endl;
    }
}

void Logger::setLogLevel(LogLevel level) {
    std::scoped_lock lock(logMutex);
    currentLogLevel = level;
}

std::string Logger::getCurrentTime() {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto zonedTime = zoned_time{ current_zone(), now };
    return format("{:%Y-%m-%d %H:%M:%S}", zonedTime.get_local_time());
}

constexpr std::string_view Logger::logLevelToString(LogLevel level) {
    switch (level) {
    case LogLevel::INFO:    return "INFO";
    case LogLevel::WARNING: return "WARNING";
    case LogLevel::ERROR:   return "ERROR";
    default:                return "UNKNOWN";
    }
}

constexpr std::string_view Logger::logCategoryToString(LogCategory category) {
    switch (category) {
    case LogCategory::CACHE:    return "CACHE";
    case LogCategory::DATABASE: return "DATABASE";
    case LogCategory::SERVICE:  return "SERVICE";
    case LogCategory::GENERAL:  return "GENERAL";
    default:                    return "UNKNOWN";
    }
}
