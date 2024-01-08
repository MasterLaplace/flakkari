/*
** EPITECH PROJECT, 2023
** Flakkari
** File description:
** Logger
*/

#include "Logger.hpp"

using namespace Flakkari;

#if defined(DEBUG) || defined(_DEBUG)
static Logger::Mode _mode = Logger::Mode::DEBUG;
#else
static Logger::Mode _mode = Logger::Mode::NORMAL;
#endif

void Logger::setMode(Logger::Mode mode) noexcept {
    _mode = mode;
}

const std::string Logger::get_current_time() noexcept
{
    auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "[ %Y-%m-%d %H:%M:%S ]", std::localtime(&currentTime));

    return std::string(buffer);
}

#ifdef _WIN32
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
#endif


void Logger::log(int level, std::string message, std::string file, int line)
{
    if (_mode == Logger::Mode::SILENT && level != LOG_FATAL)
        return;

    if (_mode == Logger::Mode::NORMAL && level == LOG_DEBUG)
        return;

    #if !defined(_WIN32) || !defined(_WIN64)
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

    #else

    std::string levelStr = "INFO";
    std::cout << get_current_time();

    switch (level) {
        case LOG_INFO:
            levelStr = "INFO";
            setColor(COLOR_CYAN);
            break;
        case LOG_LOG:
            levelStr = "LOG";
            setColor(COLOR_GREEN);
            break;
        case LOG_DEBUG:
            levelStr = "DEBUG";
            setColor(COLOR_MAGENTA);
            break;
        case LOG_WARNING:
            levelStr = "WARNING";
            setColor(COLOR_YELLOW);
            break;
        case LOG_ERROR:
            levelStr = "ERROR";
            setColor(COLOR_ORANGE);
            break;
        case LOG_FATAL:
            levelStr = "FATAL";
            setColor(COLOR_BRIGHT_RED);
            break;
    }

    std::cout << " [" << levelStr << "] " << message << " (" << file << ":" << line << ")" << std::endl;
    setColor(COLOR_RESET);
    #endif
}

void Logger::log(int level, std::string message)
{
    if (_mode == Logger::Mode::SILENT && level != LOG_FATAL)
        return;

    if (_mode == Logger::Mode::NORMAL && level == LOG_DEBUG)
        return;

    #if !defined(_WIN32) || !defined(_WIN64)
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

    #else

    std::string levelStr = "INFO";
    std::cout << get_current_time();

    switch (level) {
        case LOG_INFO:
            levelStr = "INFO";
            setColor(COLOR_CYAN);
            break;
        case LOG_LOG:
            levelStr = "LOG";
            setColor(COLOR_GREEN);
            break;
        case LOG_DEBUG:
            levelStr = "DEBUG";
            setColor(COLOR_MAGENTA);
            break;
        case LOG_WARNING:
            levelStr = "WARNING";
            setColor(COLOR_YELLOW);
            break;
        case LOG_ERROR:
            levelStr = "ERROR";
            setColor(COLOR_ORANGE);
            break;
        case LOG_FATAL:
            levelStr = "FATAL";
            setColor(COLOR_BRIGHT_RED);
            break;
    }

    std::cout << " [" << levelStr << "] " << message << std::endl;
    setColor(COLOR_RESET);
    #endif
}

void Logger::log(int level, std::string message, std::string file)
{
    if (_mode == Logger::Mode::SILENT && level != LOG_FATAL)
        return;

    if (_mode == Logger::Mode::NORMAL && level == LOG_DEBUG)
        return;

    #if !defined(_WIN32) || !defined(_WIN64)
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

    #else

    std::string levelStr = "INFO";
    std::cout << get_current_time();

    switch (level) {
        case LOG_INFO:
            levelStr = "INFO";
            setColor(COLOR_CYAN);
            break;
        case LOG_LOG:
            levelStr = "LOG";
            setColor(COLOR_GREEN);
            break;
        case LOG_DEBUG:
            levelStr = "DEBUG";
            setColor(COLOR_MAGENTA);
            break;
        case LOG_WARNING:
            levelStr = "WARNING";
            setColor(COLOR_YELLOW);
            break;
        case LOG_ERROR:
            levelStr = "ERROR";
            setColor(COLOR_ORANGE);
            break;
        case LOG_FATAL:
            levelStr = "FATAL";
            setColor(COLOR_BRIGHT_RED);
            break;
    }

    std::cout << " [" << levelStr << "] " << message << " (" << file << ")" << std::endl;
    setColor(COLOR_RESET);
    #endif
}

void Logger::log(int level, std::string message, int line)
{
    if (_mode == Logger::Mode::SILENT && level != LOG_FATAL)
        return;

    if (_mode == Logger::Mode::NORMAL && level == LOG_DEBUG)
        return;

    #if !defined(_WIN32) || !defined(_WIN64)
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

    #else

    std::string levelStr = "INFO";
    std::cout << get_current_time();

    switch (level) {
        case LOG_INFO:
            levelStr = "INFO";
            setColor(COLOR_CYAN);
            break;
        case LOG_LOG:
            levelStr = "LOG";
            setColor(COLOR_GREEN);
            break;
        case LOG_DEBUG:
            levelStr = "DEBUG";
            setColor(COLOR_MAGENTA);
            break;
        case LOG_WARNING:
            levelStr = "WARNING";
            setColor(COLOR_YELLOW);
            break;
        case LOG_ERROR:
            levelStr = "ERROR";
            setColor(COLOR_ORANGE);
            break;
        case LOG_FATAL:
            levelStr = "FATAL";
            setColor(COLOR_BRIGHT_RED);
            break;
    }

    std::cout << " [" << levelStr << "] " << message << " (" << line << ")" << std::endl;
    setColor(COLOR_RESET);
    #endif
}
