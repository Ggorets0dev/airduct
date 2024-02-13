#include "logger_class.hpp"

Logger::Logger() {}

void Logger::logError(std::string text) const
{
    std::cerr << "[-] ";
    std::cerr << text << std::endl;
}

void Logger::logMessage(std::string text) const
{
    std::clog << "[/] ";
    std::clog << text << std::endl;
}
