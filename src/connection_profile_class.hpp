#ifndef CONNECTIONPROFILE_HPP
#define CONNECTIONPROFILE_HPP

#include <iostream>
#include <regex>
#include <string>
#include <cstdio>
#include <fstream>
#include <sys/stat.h>

#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"

class ConnectionProfile
{
public:
    ConnectionProfile(); 

    static std::string dir_path;

    std::string getAddress() const  { return server_address_; };
    std::string getAlias() const    { return alias_; };
    int getPort() const             { return port_; };
    int GetBufferSize() const       { return buffer_size_; };

    bool trySetAddress(std::string& text);
    void setAlias(std::string& text);
    void setPort(const int& port);
    bool trySetBufferSize(const int& buffer_size);

    void save(const std::string& filename) const;

private:
    std::string server_address_;
    std::string alias_;
    int port_;
    int buffer_size_;
};

#endif // CONNECTIONPROFILE_HPP
