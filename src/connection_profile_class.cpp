#include "connection_profile_class.hpp"

ConnectionProfile::ConnectionProfile(const std::string& name) : name_(name) {}

bool ConnectionProfile::trySetPort(int port)
{
    if (port >= 1000 & port <= 9999)
    {
        port_ = port;
        return true;
    }
    else
    {
        return false;
    }

}

bool ConnectionProfile::trySetDetails(const std::string& text)
{
    if (!text.empty() && text.length() < 250)
    {
        details_ = text;
        return true;
    }
    else if (text.empty())
    {
        details_ = "Not provided";
        return true;
    }
    else
    {
        return false;
    }
};

bool ConnectionProfile::trySetBufferSize(int buffer_size)
{
    const int min_size(100);
    const int max_size(100'000);

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

void ConnectionProfile::fillFromCin()
{
    int buffer_integer;
    std::string buffer_string;

    std::cout << "Device port: ";
    std::cin >> buffer_integer;

    if (std::cin.fail() || !trySetPort(buffer_integer))
        throw std::runtime_error("Port could not be set, value does not meet requirements");

    std::cout << "Connection buffer size: ";
    std::cin >> buffer_integer;

    if (std::cin.fail() || !trySetBufferSize(buffer_integer))
        throw std::runtime_error("Buffer size could not be set, value does not meet requirements");

    std::cout << "Profile details: ";
    std::cin.ignore();
    std::getline(std::cin, buffer_string);

    if (!trySetDetails(buffer_string))
        throw std::runtime_error("Details could not be set, value does not meet requirements");
}
