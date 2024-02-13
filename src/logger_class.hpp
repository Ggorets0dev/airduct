#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>

class Logger
{
public:
    Logger();
    void logError(std::string text) const;
    void logMessage(std::string text) const;
};

const Logger g_main_logger;

#endif // LOGGER_HPP
