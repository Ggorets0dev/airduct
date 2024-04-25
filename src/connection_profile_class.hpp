#ifndef CONNECTIONPROFILE_HPP
#define CONNECTIONPROFILE_HPP

#include <iostream>
#include <regex>
#include <string>
#include <cstdio>
#include <fstream>
#include "filesystem.hpp"
#include "logger_class.hpp"

#include "rapidjson/document.h"

#define MIN_PORT 1024
#define MAX_PORT 65535
#define MIN_BUFFER_SIZE 100
#define MAX_BUFFER_SIZE 100000
#define MAX_DETAILS_LEN 250

enum class ConnectionSettingsSource
{
    NotSelected,
    OneTime,
    Profile
};

class ConnectionProfile
{
public:
    const std::string name_;

    ConnectionProfile(const std::string& name);
    virtual ~ConnectionProfile() {}

    /// \brief Getter for port field
    int getPort() const             { return port_; }

    /// \brief Getter for buffer_size field
    int getBufferSize() const       { return buffer_size_; }

    /// \brief Getter for details field
    std::string getDetails() const  { return details_; }

    /// \brief Setter for port field
    bool trySetPort(int port);

    /// \brief Setter for buffer_size field
    bool trySetBufferSize(int buffer_size);

    /// \brief Setter for details field
    bool trySetDetails(const std::string& text);

    ///
    /// \brief Fill object fields from string
    /// \param String with connection settings
    ///
    virtual void fillFromString(const std::string& text);

    /// \brief Fill object fields from CLI
    virtual void fillFromCin();

    /// \brief Save object in reserved directory
    virtual void save() const = 0;

    /// \brief Save object in reserved directory
    virtual void print() const = 0;

protected:
    std::string details_;
    int port_;
    int buffer_size_;
};

#endif // CONNECTIONPROFILE_HPP
