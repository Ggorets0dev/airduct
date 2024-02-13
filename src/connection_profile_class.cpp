#include "connection_profile_class.hpp"

ConnectionProfile::ConnectionProfile() {}

void ConnectionProfile::setPort(const int &port) { port_ = port; };

void ConnectionProfile::setAlias(std::string &text)
{
    if (!text.empty())
        alias_ = std::move(text);
    else
        alias_ = "Default";
};

bool ConnectionProfile::trySetAddress(std::string &text)
{
    const std::regex kAddressRegex("^((25[0-5]|(2[0-4]|1\\d|[1-9]|)\\d)\\.?\\b){4}$"); // IPv4 pattern

    if (std::regex_match(text, kAddressRegex))
    {
        server_address_ = std::move(text);
        return true;
    }
    else
    {
        return false;
    }
}

bool ConnectionProfile::trySetBufferSize(const int &buffer_size)
{
    const int min_size(100);
    const int max_size(100000);

    if (buffer_size > min_size && buffer_size < max_size)
    {
        buffer_size_ = buffer_size;
        return true;
    }
    else
    {
        return false;
    }

}
