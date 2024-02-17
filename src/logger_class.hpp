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
    void logError(const std::string&) const;
    void logMessage(const std::string& text) const;
    void logSuccess(const std::string& text) const;
};

#endif // LOGGER_HPP
