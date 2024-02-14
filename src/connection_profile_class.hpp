#ifndef CONNECTIONPROFILE_HPP
#define CONNECTIONPROFILE_HPP

#include <iostream>
#include <regex>
#include <string>
#include <cstdio>
#include <fstream>
#include <sys/stat.h>
#include "logger_class.hpp"

#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/writer.h"

class ConnectionProfile
{
public:
    static std::string dir_path;
    const std::string name_;

    static std::shared_ptr<ConnectionProfile> readFile(const std::string& path);

    ConnectionProfile(std::string name);

    std::string getAddress() const  { return address_; };
    std::string getDetails() const    { return details_; };
    int getPort() const             { return port_; };
    int getBufferSize() const       { return buffer_size_; };

    bool trySetAddress(const std::string& text);
    bool trySetDetails(const std::string& text);
    bool trySetPort(int port);
    bool trySetBufferSize(int buffer_size);

    void save(const std::string& filename) const;
    void print() const;

private:
    std::string address_;
    std::string details_;
    int port_;
    int buffer_size_;
};

#endif // CONNECTIONPROFILE_HPP
