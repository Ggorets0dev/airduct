#include "logger_class.hpp"

Logger::Logger() {}

Logger* Logger::getInstance()
{
    if (global_instance == nullptr)
        global_instance = new Logger;

    return global_instance;
}

void Logger::logError(const std::string& text) const
{
    std::cerr << "[-] ";
    std::cerr << text << std::endl;
}

void Logger::logMessage(const std::string& text) const
{
    std::clog << "[/] ";
    std::clog << text << std::endl;
}

void Logger::logSuccess(const std::string& text) const
{
    std::clog << "[+] ";
    std::clog << text << std::endl;
}
