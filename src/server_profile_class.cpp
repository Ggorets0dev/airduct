#include "server_profile_class.hpp"

void ServerProfile::save() const
{
    if (!filesystem::checkPathExist(ServerProfile::dir_path))
        filesystem::createDirectory(ServerProfile::dir_path);

    rapidjson::Document profile;
    profile.SetObject();

    rapidjson::Document::AllocatorType& alloc = profile.GetAllocator();

    profile.AddMember("name", name_, alloc);
    profile.AddMember("type", "SERVER", alloc);
    profile.AddMember("details", details_, alloc);
    profile.AddMember("port", port_, alloc);
    profile.AddMember("buffer_size", buffer_size_, alloc);

    const std::string kFilePath = std::string(dir_path) + "/" + name_ + ".json";
    filesystem::saveJson(profile, kFilePath.c_str());

    Logger::getInstance()->logSuccess("Client profile with name " + name_ + " saved with path " + kFilePath);
}

void ServerProfile::print() const
{
    std::cout << "Profile: " << name_ << std::endl;
    std::cout << "Type: [SERVER]" << std::endl;
    std::cout << "Port: " << port_ << std::endl;
    std::cout << "Buffer size: " << buffer_size_ << std::endl;
    std::cout << "Details: " << details_ << std::endl;
}

std::shared_ptr<ServerProfile> ServerProfile::createFromFile(const std::string& path)
{
    auto profile_file = filesystem::readJson(path);

    if (profile_file == nullptr)
        return nullptr;

    bool has_name = (*profile_file).HasMember("name") && (*profile_file)["name"].IsString();
    bool has_port = (*profile_file).HasMember("port") && (*profile_file)["port"].IsInt();
    bool has_buffer_size = (*profile_file).HasMember("buffer_size") && (*profile_file)["buffer_size"].IsInt();
    bool has_details = (*profile_file).HasMember("details") && (*profile_file)["details"].IsString();

    if (!has_name || !has_port || !has_buffer_size || !has_details)
    {
        Logger::getInstance()->logError("JSON structure of the object in the file at path " + path + " does not correspond to the profile structure");
        return nullptr;
    }

    bool create_success;
    std::shared_ptr<ServerProfile> profile(new ServerProfile((*profile_file)["name"].GetString()));

    // NOTE - Try set details
    std::string details = (*profile_file)["details"].GetString();
    create_success *= (*profile).trySetDetails(details);

    // NOTE - Try set port
    (*profile).trySetPort((*profile_file)["port"].GetInt());

    // NOTE - Try set buffer size
    (*profile).trySetBufferSize((*profile_file)["buffer_size"].GetInt());

    return profile;
}

std::string ServerProfile::getFilePath(const std::string& profile_name)
{
    std::string path = dir_path;
    path += "/";
    path += profile_name;
    path += ".json";

    return path;
}
