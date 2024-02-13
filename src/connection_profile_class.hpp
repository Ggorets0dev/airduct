#ifndef CONNECTIONPROFILE_HPP
#define CONNECTIONPROFILE_HPP

#include <string>
#include <regex>

class ConnectionProfile
{
public:
    ConnectionProfile(); 

    std::string getAddress() const  { return server_address_; };
    std::string getAlias() const    { return alias_; };
    int getPort() const             { return port_; };
    int GetBufferSize() const       { return buffer_size_; };

    bool trySetAddress(std::string& text);
    void setAlias(std::string& text);
    void setPort(const int& port);
    bool trySetBufferSize(const int& buffer_size);

private:
    std::string server_address_;
    std::string alias_;
    int port_;
    int buffer_size_;
};

#endif // CONNECTIONPROFILE_HPP
