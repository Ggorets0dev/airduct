#include <iostream>
#include <getopt.h>
#include <vector>
#include <string>
#include <functional>
#include "project_info.h"
#include "version.h"
#include "filesystem.hpp"
#include "web_server_class.hpp"
#include "web_client_class.hpp"
#include "logger_class.hpp"
#include "client_profile_class.hpp"
#include "server_profile_class.hpp"

Logger* Logger::global_instance = nullptr;
const char* ClientProfile::dir_path = "./client_profiles";
const char* ServerProfile::dir_path = "./server_profiles";

const option longOptions[] = {
    { "version", no_argument, nullptr, 'v' },
    { "help", no_argument, nullptr, 'v' },
    { "listen", no_argument, nullptr, 'v' },
    { "create", required_argument, nullptr, 'c' },
    { "show", required_argument, nullptr, 's' },
    { "execute", required_argument, nullptr, 'e' },
    { "remove", required_argument, nullptr, 'r' },
    { "destination", required_argument, nullptr, 'd' },
    { "profile", required_argument, nullptr, 'p' },
    { nullptr, 0, nullptr, 0 }
};

// SECTION - Auxiliary functions for processing command line arguments
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
    std::string server_profile_path = ServerProfile::getFilePath(profile_name);
    std::string client_profile_path = ClientProfile::getFilePath(profile_name);

    bool is_deleted_as_server = checkPathExist(server_profile_path.c_str()) && removeFile(server_profile_path.c_str());
    bool is_deleted_as_client = checkPathExist(client_profile_path.c_str()) && removeFile(client_profile_path.c_str());

    if (is_deleted_as_server || is_deleted_as_client)
        Logger::getInstance()->logSuccess("Successfully deleted profile on path " + std::string(profile_name));
    else
        Logger::getInstance()->logError("Failed to delete the profile on path " + std::string(profile_name) + " due to its unavailability or an error in operation");
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

    if (!client_paths.empty())
        std::cout << std::endl;

    show(server_paths, ServerProfile::createFromFile);
}

std::shared_ptr<ClientProfile> collectProfile(const std::string& profile_name)
{
    std::string profile_path;

    if (!checkPathExist(profile_path.c_str()))
    {
        Logger::getInstance()->logError("Failed to locate the requested profile on the intended path " + profile_path);
        return nullptr;
    }

    return nullptr;
}

std::shared_ptr<ClientProfile> parseConnectionSettings(const char* connection_settings)
{
    const std::string params = connection_settings; // format is "address:port@buffer_size"

    std::size_t colon_inx = params.find(':');
    std::size_t ad_inx = params.find('@');

    if (colon_inx == std::string::npos || ad_inx == std::string::npos)
    {
        Logger::getInstance()->logError("Unable to process connection settings due to format mismatch");
        return nullptr;
    }

    auto profile = std::make_shared<ClientProfile>("TEMP");

    try
    {
        std::string address = std::string(params.cbegin(), params.cbegin() + colon_inx);
        int port = std::stoi(std::string(params.cbegin() + colon_inx + 1, params.cbegin() + ad_inx));
        int buffer_size = std::stoi(std::string(params.cbegin() + ad_inx + 1, params.cend()));

         if (profile.get()->trySetAddress(address) &&
                 profile.get()->trySetPort(port) &&
                 profile.get()->trySetBufferSize(buffer_size))
         {
             return profile;
         }
         else
         {
             Logger::getInstance()->logError("Failed to assign one of the parameters for the connection profile");
             return nullptr;
         }
    }
    catch (std::invalid_argument)
    {
        Logger::getInstance()->logError("Failed to convert part of the transferred connection settings");
        return nullptr;
    }
}

void printSoftwareInformation()
{
    std::cout << "AirDuct v" << g_version << std::endl;
    std::cout << "Developer: " << g_developer << std::endl;
    std::cout << "Repository: " << g_repository << std::endl;
    std::cout << "License: " << g_license << std::endl;
}

void printHelp()
{

}
// !SECTION

int main(int argc, char** argv)
{
    std::unique_ptr<WebClient> web_client;
    std::unique_ptr<WebServer> web_server;

    int port;
    std::shared_ptr<ClientProfile> client_profile;
    std::shared_ptr<ServerProfile> server_profile;
    std::string command;

    std::string buffer; // For different operations in switch-case statement

    bool isExecuteCalled(false), isListenCalled(false), isDestinationSet(false), isProfileSet(false);

    int option, optinx;
    while ((option = getopt_long(argc, argv, "e:d:c:r:s:p:vhl", longOptions, &optinx)) != -1)
    {
        switch (option)
        {
        case 'h':
            printHelp();
            break;

        case 'v':
            printSoftwareInformation();
            break;

        case 'c':
            createProfile(optarg);
            break;

        case 'r':
            removeProfile(optarg);
            break;

        case 's':
            displayProfile(optarg);
            break;

        case 'e':
            command = optarg;
            isExecuteCalled = true;
            break;

        case 'l':
            isListenCalled = true;
            break;

        case 'p':
            buffer = optarg; // Saving profile name
            client_profile = collectProfile(buffer);

            if (client_profile != nullptr)
            {
                web_client = std::make_unique<WebClient>(*client_profile);
                isProfileSet = true;
                Logger::getInstance()->logSuccess("Connection profile with name " + buffer + " is successfully obtained");
            }
            else
            {
                Logger::getInstance()->logError("Unable to continue profile assignment due to file read error");
            }
            break;

        case 'd':
            if (isListenCalled) // NOTE - Requires assignment of connection settings for the connection
            {
                client_profile = parseConnectionSettings(optarg);

                if (client_profile != nullptr)
                {
                    web_client = std::make_unique<WebClient>(*client_profile);
                    isDestinationSet = true;
                    Logger::getInstance()->logSuccess("Settings string has been successfully processed and a temporary connection profile has been created");
                }
                else
                {
                    Logger::getInstance()->logError("Unable to continue profile creation due to arguments read error");
                }
            }
            else if (isExecuteCalled) // NOTE - Requires port definition for server creation
            {
                port = atoi(optarg);
            }
            else
            {
                Logger::getInstance()->logError("Operation for which the assignment is to be set is not defined");
            }
            break;

        default:
            Logger::getInstance()->logError("Unrecognized argument received");
            return 1;
        }
    }

    // SECTION - Processing listen (recieve) operation
    if (isListenCalled && (isProfileSet || isDestinationSet))
    {

    }
    else if (isListenCalled)
    {
        Logger::getInstance()->logError("To listen transfered data, you must either select a connection profile or specify the settings manually");
    }
    // !SECTION

    // SECTION - Processing execute (send) operation
    if (isExecuteCalled)
    {
        //web_server = std::make_unique<WebServer>();
    }
    // !SECTION

    return 0;
}
