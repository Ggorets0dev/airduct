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

void ConnectionProfile::save(const std::string& filename) const
{
    struct stat sb;

    if (stat(dir_path.c_str(), &sb) != 0)
        mkdir(dir_path.c_str(), 0777);

    rapidjson::Document profile;
    profile.SetObject();

    rapidjson::Document::AllocatorType& alloc = profile.GetAllocator();

    profile.AddMember("server_address", server_address_, alloc);
    profile.AddMember("alias", alias_, alloc);
    profile.AddMember("port", port_, alloc);
    profile.AddMember("buffer_size", buffer_size_, alloc);

    const std::string kFilePath = dir_path + "/" + filename;
    FILE* file = fopen(kFilePath.c_str(), "w");

    char writeBuffer[65536];
    rapidjson::FileWriteStream os(file, writeBuffer, sizeof(writeBuffer));
    rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
    profile.Accept(writer);

    fclose(file);
}
