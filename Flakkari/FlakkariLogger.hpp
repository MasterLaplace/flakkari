/*
** EPITECH PROJECT, 2023
** Flakkari
** File description:
** Flakkari::Logger
*/

#ifndef FLAKKARI_LOGGER_HPP_
    #define FLAKKARI_LOGGER_HPP_

#define LOG_INFO 0
#define LOG_LOG 1
#define LOG_DEBUG 2
#define LOG_WARNING 3
#define LOG_ERROR 4
#define LOG_FATAL 5

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>

#define FLAKKARI_LOG(level, message) Flakkari::Logger::log(level, message, __FILE__, __LINE__)
#define FLAKKARI_LOG_INFO(message) FLAKKARI_LOG(LOG_INFO, message)
#define FLAKKARI_LOG_LOG(message) FLAKKARI_LOG(LOG_LOG, message)
#define FLAKKARI_LOG_DEBUG(message) FLAKKARI_LOG(LOG_DEBUG, message)
#define FLAKKARI_LOG_WARNING(message) FLAKKARI_LOG(LOG_WARNING, message)
#define FLAKKARI_LOG_ERROR(message) FLAKKARI_LOG(LOG_ERROR, message)
#define FLAKKARI_LOG_FATAL(message) FLAKKARI_LOG(LOG_FATAL, message)

#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"
#define COLOR_WHITE "\033[37m"
#define COLOR_ORANGE "\033[38;5;208m"
#define COLOR_BRIGHT_RED "\033[91m"
#define COLOR_BRIGHT_GREEN "\033[92m"
#define COLOR_BRIGHT_YELLOW "\033[93m"
#define COLOR_BRIGHT_BLUE "\033[94m"
#define COLOR_BRIGHT_MAGENTA "\033[95m"
#define COLOR_BRIGHT_CYAN "\033[96m"
#define COLOR_BRIGHT_WHITE "\033[97m"

namespace Flakkari {
    class Logger {
        public:
            static const std::string get_current_time() noexcept;
            static void log(int level, std::string message, std::string file, int line);
            static void log(int level, std::string message);
            static void log(int level, std::string message, std::string file);
            static void log(int level, std::string message, int line);
    };
} // namespace Flakkari

#endif /* !FLAKKARI_LOGGER_HPP_ */

#ifdef FLAKKARI_LOGGER_IMPLEMENTATION

const std::string Flakkari::Logger::get_current_time() noexcept
{
    auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "[ %Y-%m-%d %H:%M:%S ]", std::localtime(&currentTime));

    return std::string(buffer);
}

void Flakkari::Logger::log(int level, std::string message, std::string file, int line)
{
    std::string color = COLOR_RESET;
    std::string levelStr = "INFO";

    switch (level) {
        case LOG_INFO:
            color = COLOR_CYAN;
            levelStr = "INFO";
            break;
        case LOG_LOG:
            color = COLOR_GREEN;
            levelStr = "LOG";
            break;
        case LOG_DEBUG:
            color = COLOR_MAGENTA;
            levelStr = "DEBUG";
            break;
        case LOG_WARNING:
            color = COLOR_YELLOW;
            levelStr = "WARNING";
            break;
        case LOG_ERROR:
            color = COLOR_ORANGE;
            levelStr = "ERROR";
            break;
        case LOG_FATAL:
            color = COLOR_BRIGHT_RED;
            levelStr = "FATAL";
            break;
    }

    std::cout << get_current_time();
    std::cout << color << " [" << levelStr << "] " << message << " (" << file << ":" << line << ")" << COLOR_RESET << std::endl;
}

void Flakkari::Logger::log(int level, std::string message)
{
    std::string color = COLOR_RESET;
    std::string levelStr = "INFO";

    switch (level) {
        case LOG_INFO:
            color = COLOR_CYAN;
            levelStr = "INFO";
            break;
        case LOG_LOG:
            color = COLOR_GREEN;
            levelStr = "LOG";
            break;
        case LOG_DEBUG:
            color = COLOR_MAGENTA;
            levelStr = "DEBUG";
            break;
        case LOG_WARNING:
            color = COLOR_YELLOW;
            levelStr = "WARNING";
            break;
        case LOG_ERROR:
            color = COLOR_ORANGE;
            levelStr = "ERROR";
            break;
        case LOG_FATAL:
            color = COLOR_BRIGHT_RED;
            levelStr = "FATAL";
            break;
    }

    std::cout << get_current_time();
    std::cout << color << " [" << levelStr << "] " << message << COLOR_RESET << std::endl;
}

void Flakkari::Logger::log(int level, std::string message, std::string file)
{
    std::string color = COLOR_RESET;
    std::string levelStr = "INFO";

    switch (level) {
        case LOG_INFO:
            color = COLOR_CYAN;
            levelStr = "INFO";
            break;
        case LOG_LOG:
            color = COLOR_GREEN;
            levelStr = "LOG";
            break;
        case LOG_DEBUG:
            color = COLOR_MAGENTA;
            levelStr = "DEBUG";
            break;
        case LOG_WARNING:
            color = COLOR_YELLOW;
            levelStr = "WARNING";
            break;
        case LOG_ERROR:
            color = COLOR_ORANGE;
            levelStr = "ERROR";
            break;
        case LOG_FATAL:
            color = COLOR_BRIGHT_RED;
            levelStr = "FATAL";
            break;
    }

    std::cout << get_current_time();
    std::cout << color << " [" << levelStr << "] " << message << " (" << file << ")" << COLOR_RESET << std::endl;
}

void Flakkari::Logger::log(int level, std::string message, int line)
{
    std::string color = COLOR_RESET;
    std::string levelStr = "INFO";

    switch (level) {
        case LOG_INFO:
            color = COLOR_CYAN;
            levelStr = "INFO";
            break;
        case LOG_LOG:
            color = COLOR_GREEN;
            levelStr = "LOG";
            break;
        case LOG_DEBUG:
            color = COLOR_MAGENTA;
            levelStr = "DEBUG";
            break;
        case LOG_WARNING:
            color = COLOR_YELLOW;
            levelStr = "WARNING";
            break;
        case LOG_ERROR:
            color = COLOR_ORANGE;
            levelStr = "ERROR";
            break;
        case LOG_FATAL:
            color = COLOR_BRIGHT_RED;
            levelStr = "FATAL";
            break;
    }

    std::cout << get_current_time();
    std::cout << color << " [" << levelStr << "] " << message << " (" << line << ")" << COLOR_RESET << std::endl;
}

#endif /* !FLAKKARI_LOGGER_IMPLEMENTATION */
