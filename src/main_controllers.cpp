#include "main_controllers.hpp"

void printHelp()
{
    std::cout << "Available commands for AirDuct:" << std::endl;

    std::string buffer = "--version, -v [Display version and other software information]\n";
    buffer += "--help, -h [Display available commands for software]\n";
    buffer += "--create, -c <client | server> [Create a new profile for this direction]\n";
    buffer += "--show, -s <all | server | client | PROFILE_NAME> [Display profiles belonging to a specific category or one specific one]\n";
    buffer += "--execute, -e <COMMAND> [Execute the command either as a source (send mode) or as a post-processing (receive mode) command]\n";
    buffer += "--remove, -r <all | server | client | PROFILE_NAME> [Delete either all profiles from a category or one specific one]\n";
    buffer += "--destination, -d <SETTINGS_STRING> [Set connection settings without profile (one-time)]\n";
    buffer += "--profile, -p <PROFILE_NAME> [Select a profile by name to use in the connection]\n";
    buffer += "--transmit, -t [Select the data transfer mode]\n";
    buffer += "--obtain, -o [Select the data recieve mode]";

    std::cout << buffer << std::endl;
    std::cout << std::endl;
    std::cout << "Format of connection settings:" << std::endl;

    buffer = "Client: IP:PORT/BUFFER_SIZE\n";
    buffer += "Server: PORT/BUFFER_SIZE\n";
    buffer += "Note: IP pattern is IPv4 (a specific host address, not a network or broadcast address)\n";
    buffer += "Note: Port limitations are " + std::to_string(MIN_PORT) + " - " + std::to_string(MAX_PORT) + "\n";
    buffer += "Note: Buffer size limitations are " + std::to_string(MIN_BUFFER_SIZE) + " - " + std::to_string(MAX_BUFFER_SIZE);
    std::cout << buffer << std::endl;
    std::cout << std::endl;
    std::cout << "Additional information:" << std::endl;

    buffer = "*Buffer size* - Number of characters to be transmitted/received, choosing a number with a margin will slightly increase memory consumption but guarantees data integrity";

    std::cout << buffer << std::endl;
}

void createProfile(const char* type)
{
    NetworkMode mode {NetworkMode::NotSelected};

    std::string profile_name;
    ConnectionProfile* new_profile;

    if (strcmp(type, "client") == 0)
        mode = NetworkMode::Client;
    else if (strcmp(type, "server") == 0)
        mode = NetworkMode::Server;
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
        if (mode == NetworkMode::Client)
        {
            new_profile = new ClientProfile(profile_name);
            new_profile->fillFromCin();
        }
        else if ((mode == NetworkMode::Server))
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
        filesystem::removeDirFiles(ClientProfile::dir_path);
        filesystem::removeDirFiles(ServerProfile::dir_path);
    }
    else if (strcmp(profile_name, "client") == 0)
    {
        filesystem::removeDirFiles(ClientProfile::dir_path);
    }
    else if (strcmp(profile_name, "server") == 0)
    {
        filesystem::removeDirFiles(ServerProfile::dir_path);
    }
    else
    {
        std::string server_profile_path = ServerProfile::getFilePath(profile_name);
        std::string client_profile_path = ClientProfile::getFilePath(profile_name);

        bool is_deleted_as_server = filesystem::checkPathExist(server_profile_path.c_str()) && filesystem::removeFile(server_profile_path.c_str());
        bool is_deleted_as_client = filesystem::checkPathExist(client_profile_path.c_str()) && filesystem::removeFile(client_profile_path.c_str());

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
        client_paths = filesystem::getFilePaths(ClientProfile::dir_path);
        server_paths = filesystem::getFilePaths(ServerProfile::dir_path);
    }
    else if (strcmp(profile_name, "client") == 0)
    {
        client_paths = filesystem::getFilePaths(ClientProfile::dir_path);
    }
    else if (strcmp(profile_name, "server") == 0)
    {
        server_paths = filesystem::getFilePaths(ServerProfile::dir_path);
    }
    else
    {
        auto cl_path = ClientProfile::getFilePath(profile_name);
        auto se_path = ServerProfile::getFilePath(profile_name);

        if (filesystem::checkPathExist(cl_path.c_str()))
            client_paths.push_back(cl_path);
        else if (filesystem::checkPathExist(se_path.c_str()))
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

    if (filesystem::checkPathExist(client_path.c_str()))
    {
        return ClientProfile::createFromFile(client_path);
    }
    else if (filesystem::checkPathExist(server_path.c_str()))
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
