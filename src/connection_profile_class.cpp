#include "connection_profile_class.hpp"

ConnectionProfile::ConnectionProfile(const std::string& name) : name_(name) {}

bool ConnectionProfile::trySetPort(int port)
{
    if (port >= MIN_PORT & port <= MAX_PORT)
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
    if (!text.empty() && text.length() < MAX_DETAILS_LEN)
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
}

bool ConnectionProfile::trySetBufferSize(int buffer_size)
{
    if (buffer_size > MIN_BUFFER_SIZE && buffer_size < MAX_BUFFER_SIZE)
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

void ConnectionProfile::fillFromString(const std::string& text)
{
    std::size_t slash_inx = text.find('/');

    if (slash_inx == std::string::npos)
        throw std::invalid_argument("Data cannot be processed because the format is not followed");

    int port = std::stoi(std::string(text.cbegin(), text.cbegin() + slash_inx));
    int buffer_size = std::stoi(std::string(text.cbegin() + slash_inx + 1, text.cend()));

    if (!trySetPort(port))
        throw std::invalid_argument("Port could not be set, value does not meet requirements");

    if (!trySetBufferSize(buffer_size))
        throw std::invalid_argument("Buffer size could not be set, value does not meet requirements");
}
