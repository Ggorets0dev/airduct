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

    ///
    /// \brief Output a message to CERR with error mark
    /// \param text - Log message
    ///
    void logError(const std::string& text) const;

    ///
    /// \brief Output a message to COUT with information mark
    /// \param text - Log message
    ///
    void logMessage(const std::string& text) const;

    ///
    /// \brief Output a message to COUT with success mark
    /// \param text - Log message
    ///
    void logSuccess(const std::string& text) const;
};

#endif // LOGGER_HPP
