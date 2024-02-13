#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>

class Logger
{
public:
    static Logger* global_instance;
    static Logger* getInstance();

    Logger(const Logger& other) = delete;
    void operator = (const Logger& other) = delete;

    Logger();
    void logError(std::string text) const;
    void logMessage(std::string text) const;
    void logSuccess(std::string text) const;
};

#endif // LOGGER_HPP
