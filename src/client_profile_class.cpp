#include "client_profile_class.hpp"

#include <iostream>

std::shared_ptr<ClientProfile> ClientProfile::createFromFile(const std::string& path)
{
    auto profile_file = filesystem::readJson(path);

    if (profile_file == nullptr)
        return nullptr;

    bool has_name = profile_file->HasMember("name") && (*profile_file)["name"].IsString();
    bool has_address = profile_file->HasMember("address") && (*profile_file)["address"].IsString();
    bool has_port = profile_file->HasMember("port") && (*profile_file)["port"].IsInt();
    bool has_buffer_size = profile_file->HasMember("buffer_size") && (*profile_file)["buffer_size"].IsInt();
    bool has_details = profile_file->HasMember("details") && (*profile_file)["details"].IsString();

    if (!has_name || !has_address || !has_port || !has_buffer_size || !has_details)
    {
        Logger::getInstance()->logError("JSON structure of the object in the file at path " + path + " does not correspond to the profile structure");
        return nullptr;
    }

    bool create_success;
    std::string buffer;
    std::shared_ptr<ClientProfile> profile = std::make_shared<ClientProfile>((*profile_file)["name"].GetString());

    // NOTE - Try set address
    buffer = (*profile_file)["address"].GetString();
    create_success *= profile->trySetAddress(buffer);

    // NOTE - Try set details
    buffer = (*profile_file)["details"].GetString();
    create_success *= profile->trySetDetails(buffer);

    // NOTE - Try set port
    profile->trySetPort((*profile_file)["port"].GetInt());

    // NOTE - Try set buffer size
    profile->trySetBufferSize((*profile_file)["buffer_size"].GetInt());

    return profile;
}

bool ClientProfile::trySetAddress(const std::string& text)
{
    const std::regex kAddressRegex("^((25[0-5]|(2[0-4]|1\\d|[1-9]|)\\d)\\.?\\b){4}$"); // IPv4 pattern

    if (std::regex_match(text, kAddressRegex))
    {
        address_ = text;
        return true;
    }
    else
    {
        return false;
    }
}

void ClientProfile::print() const
{
    std::cout << "Profile: " << name_ << std::endl;
    std::cout << "Type: [CLIENT]" << std::endl;
    std::cout << "Address: " << address_ << std::endl;
    std::cout << "Port: " << port_ << std::endl;
    std::cout << "Buffer size: " << buffer_size_ << std::endl;
    std::cout << "Details: " << details_ << std::endl;
}

void ClientProfile::save() const
{
    if (!filesystem::checkPathExist(ClientProfile::dir_path))
        filesystem::createDirectory(ClientProfile::dir_path);

    rapidjson::Document profile;
    profile.SetObject();

    rapidjson::Document::AllocatorType& alloc = profile.GetAllocator();

    profile.AddMember("name", name_, alloc);
    profile.AddMember("type", "CLIENT", alloc);
    profile.AddMember("address", name_, alloc);
    profile.AddMember("details", details_, alloc);
    profile.AddMember("port", port_, alloc);
    profile.AddMember("buffer_size", buffer_size_, alloc);

    const std::string kFilePath = std::string(dir_path) + "/" + name_ + ".json";

    filesystem::saveJson(profile, kFilePath.c_str());

    Logger::getInstance()->logSuccess("Client profile with name " + name_ + " saved with path " + kFilePath);
}

std::string ClientProfile::getFilePath(const std::string& profile_name)
{
    std::string path = dir_path;
    path += "/";
    path += profile_name;
    path += ".json";

    return path;
}

void ClientProfile::fillFromCin()
{
    std::string address;

    std::cout << "Device IPv4 address: ";
    std::cin >> address;

    if (!trySetAddress(address))
        throw std::runtime_error("Address could not be set, value does not meet requirements");

    ConnectionProfile::fillFromCin();
}

void ClientProfile::fillFromString(const std::string &text)
{
    std::size_t colon_inx = text.find(':');

    if (colon_inx == std::string::npos || colon_inx > text.size())
        throw std::invalid_argument("Data cannot be processed because the format is not followed");

    std::string address = std::string(text.cbegin(), text.cbegin() + colon_inx);

    if (!trySetAddress(address))
        throw std::invalid_argument("Address could not be set, value does not meet requirements");

    ConnectionProfile::fillFromString(text.substr(colon_inx + 1, text.size() - colon_inx));
}
