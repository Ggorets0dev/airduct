#include "connection_profile_class.hpp"

ConnectionProfile::ConnectionProfile(std::string name) : name_(name) {}

std::shared_ptr<ConnectionProfile> ConnectionProfile::readFile(const std::string& path)
{
    FILE* fp = fopen(path.c_str(), "rb");

    // NOTE - Check if the file was opened successfully
    if (!fp)
    {
        Logger::getInstance()->logError("Failed to open file at path " + path + " to read JSON object");
        return nullptr;
    }

    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    rapidjson::Document profile_file;
    profile_file.ParseStream(is);

    // NOTE - Check if the document is valid
    if (profile_file.HasParseError())
    {
        Logger::getInstance()->logError("Failed to parse JSON object in file at path " + path);
        fclose(fp);

        return nullptr;
    }

    fclose(fp);

    bool has_name = profile_file.HasMember("name") && profile_file["name"].IsString();
    bool has_address = profile_file.HasMember("address") && profile_file["address"].IsString();
    bool has_port = profile_file.HasMember("port") && profile_file["port"].IsInt();
    bool has_buffer_size = profile_file.HasMember("buffer_size") && profile_file["buffer_size"].IsInt();
    bool has_details = profile_file.HasMember("details") && profile_file["details"].IsString();

    if (!has_name || !has_address || !has_port || !has_buffer_size || !has_details)
    {
        Logger::getInstance()->logError("JSON structure of the object in the file at path " + path + " does not correspond to the profile structure");
        return nullptr;
    }

    bool create_success;
    std::string buffer;
    std::shared_ptr<ConnectionProfile> profile(new ConnectionProfile(profile_file["name"].GetString()));

    // NOTE - Try set address
    buffer = profile_file["address"].GetString();
    create_success *= (*profile).trySetAddress(buffer);

    // NOTE - Try set details
    buffer = profile_file["details"].GetString();
    create_success *= (*profile).trySetDetails(buffer);

    // NOTE - Try set port
    (*profile).trySetPort(profile_file["port"].GetInt());

    // NOTE - Try set buffer size
    (*profile).trySetBufferSize(profile_file["buffer_size"].GetInt());

    return profile;
}

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
    else
    {
        return false;
    }
};

bool ConnectionProfile::trySetAddress(const std::string& text)
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

bool ConnectionProfile::trySetBufferSize(int buffer_size)
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

    profile.AddMember("name", name_, alloc);
    profile.AddMember("address", address_, alloc);
    profile.AddMember("details", details_, alloc);
    profile.AddMember("port", port_, alloc);
    profile.AddMember("buffer_size", buffer_size_, alloc);

    const std::string kFilePath = dir_path + "/" + filename;
    FILE* file = fopen(kFilePath.c_str(), "w");

    char writeBuffer[65536];
    rapidjson::FileWriteStream os(file, writeBuffer, sizeof(writeBuffer));
    rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
    profile.Accept(writer);

    fclose(file);

    Logger::getInstance()->logSuccess("Profile with name " + name_ + " saved with path " + kFilePath);
}

void ConnectionProfile::print() const
{
    std::cout << "Profile: " << name_ << std::endl;
    std::cout << "Server address: " << address_ << std::endl;
    std::cout << "Server port: " << port_ << std::endl;
    std::cout << "Buffer size: " << buffer_size_ << std::endl;
    std::cout << "Details: " << details_ << std::endl;
}
