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

class ConnectionProfile
{
public:
    const std::string name_;

    ConnectionProfile(const std::string& name);
    virtual ~ConnectionProfile() {};

    int getPort() const             { return port_; };
    int getBufferSize() const       { return buffer_size_; };
    std::string getDetails() const  { return details_; };

    bool trySetPort(int port);
    bool trySetBufferSize(int buffer_size);
    bool trySetDetails(const std::string& text);

    virtual void fillFromCin();
    virtual void save() const = 0;
    virtual void print() const = 0;

protected:
    std::string details_;
    int port_;
    int buffer_size_;
};

#endif // CONNECTIONPROFILE_HPP
