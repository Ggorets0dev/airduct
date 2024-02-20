#include "main_controllers.h"

void createProfile(const char* type)
{
    bool isClient(false), isServer(false);
    std::string profile_name;
    ConnectionProfile* new_profile;

    if (strcmp(type, "client") == 0)
        isClient = true;
    else if (strcmp(type, "server") == 0)
        isServer = true;
    else
        return Logger::getInstance()->logError("No specified connection profile type exists");

    std::cout << "Profile name: ";
    std::cin >> profile_name;

    // SECTION - Check for blocked names
    auto profile_name_lower = profile_name;
    std::for_each(profile_name_lower.begin(), profile_name_lower.end(), [](char& c){ return std::tolower(c); });

    if (profile_name_lower == "client" || profile_name_lower == "server")
        return Logger::getInstance()->logError("Names \"server\" and \"client\" in any case are reserved and cannot be used as a profile name");
    // !SECTION

    try
    {
        if (isClient)
        {
            new_profile = new ClientProfile(profile_name);
            new_profile->fillFromCin();
        }
        else
        {
            new_profile = new ServerProfile(profile_name);
            new_profile->fillFromCin();
        }
    }
    catch (std::runtime_error&)
    {
        delete new_profile;
        return Logger::getInstance()->logError("Failed to complete profile fields due to incorrectly entered values");
    }

    new_profile->save();

    std::cout << "Following profile was created: " << std::endl;
    new_profile->print();

    delete new_profile;
}

void removeProfile(const char* profile_name)
{
    if (strcmp(profile_name, "all") == 0)
    {
        removeDirFiles(ClientProfile::dir_path);
        removeDirFiles(ServerProfile::dir_path);
    }
    else if (strcmp(profile_name, "client") == 0)
    {
        removeDirFiles(ClientProfile::dir_path);
    }
    else if (strcmp(profile_name, "server") == 0)
    {
        removeDirFiles(ServerProfile::dir_path);
    }
    else
    {
        std::string server_profile_path = ServerProfile::getFilePath(profile_name);
        std::string client_profile_path = ClientProfile::getFilePath(profile_name);

        bool is_deleted_as_server = checkPathExist(server_profile_path.c_str()) && removeFile(server_profile_path.c_str());
        bool is_deleted_as_client = checkPathExist(client_profile_path.c_str()) && removeFile(client_profile_path.c_str());

        if (is_deleted_as_server || is_deleted_as_client)
            Logger::getInstance()->logSuccess("Successfully deleted profile on path " + std::string(profile_name));
        else
            Logger::getInstance()->logError("Failed to delete the profile on path " + std::string(profile_name) + " due to its unavailability or an error in operation");
    }
}

void displayProfile(const char* profile_name)
{
    std::vector<std::string> client_paths;
    std::vector<std::string> server_paths;

    if (strcmp(profile_name, "all") == 0)
    {
        client_paths = getFilePaths(ClientProfile::dir_path);
        server_paths = getFilePaths(ServerProfile::dir_path);
    }
    else if (strcmp(profile_name, "client") == 0)
    {
        client_paths = getFilePaths(ClientProfile::dir_path);
    }
    else if (strcmp(profile_name, "server") == 0)
    {
        server_paths = getFilePaths(ServerProfile::dir_path);
    }
    else
    {
        auto cl_path = ClientProfile::getFilePath(profile_name);
        auto se_path = ServerProfile::getFilePath(profile_name);

        if (checkPathExist(cl_path.c_str()))
            client_paths.push_back(cl_path);
        else if (checkPathExist(se_path.c_str()))
            server_paths.push_back(cl_path);
    }

    if (client_paths.empty() && server_paths.empty())
        return Logger::getInstance()->logError("Profiles cannot be displayed because they do exist");

    // SECTION - Function for showing results
    using StringArray = std::vector<std::string>;
    using SharedPtr = std::shared_ptr<ConnectionProfile>;

    auto show = [](StringArray& paths, std::function<SharedPtr(const std::string& path)> create)
    {
        for (auto iter = paths.cbegin(); iter != paths.cend(); ++iter)
        {
            auto elem = create(*iter);

            if (elem != nullptr)
            {
                elem->print();

                if (iter != paths.cend() - 1)
                    std::cout << std::endl;
            }
        }
    };
    // !SECTION

    show(client_paths, ClientProfile::createFromFile);

    if (!client_paths.empty() && !server_paths.empty())
        std::cout << std::endl;

    show(server_paths, ServerProfile::createFromFile);
}

std::shared_ptr<ConnectionProfile> collectProfile(const std::string& profile_name)
{
    auto client_path = ClientProfile::getFilePath(profile_name);
    auto server_path = ServerProfile::getFilePath(profile_name);

    if (checkPathExist(client_path.c_str()))
    {
        return ClientProfile::createFromFile(client_path);
    }
    else if (checkPathExist(server_path.c_str()))
    {
        return ServerProfile::createFromFile(server_path);
    }
    else
    {
        Logger::getInstance()->logError("Failed to locate the requested profile");
        return nullptr;
    }
}

std::shared_ptr<ConnectionProfile> parseConnectionSettings(const std::string& params)
{
    std::shared_ptr<ConnectionProfile> profile = std::make_shared<ClientProfile>("TEMP");

    try
    {
        profile->fillFromString(params);
        return profile;
    }
    catch (std::invalid_argument&)
    {
        profile.reset(new ServerProfile("TEMP"));
    }

    try
    {
        profile->fillFromString(params);
        return profile;
    }
    catch (std::invalid_argument&)
    {
        Logger::getInstance()->logError("Failed to process the settings string and bring it to any profile due to format non-compliance");
        return nullptr;
    }
}
