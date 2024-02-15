#include <iostream>
#include <getopt.h>
#include <vector>
#include <string>
#include "project_info.h"
#include "version.h"
#include "filesystem.hpp"
#include "web_server_class.hpp"
#include "web_client_class.hpp"
#include "logger_class.hpp"
#include "connection_profile_class.hpp"

Logger* Logger::global_instance = nullptr;
const char* ConnectionProfile::dir_path = "./profiles";

const option longOptions[] = {
    { "version", no_argument, nullptr, 'v' },
    { "help", no_argument, nullptr, 'v' },
    { "create", required_argument, nullptr, 'c' },
    { "show", required_argument, nullptr, 's' },
    { "execute", required_argument, nullptr, 'e' },
    { "remove", required_argument, nullptr, 'r' },
    { "destination", required_argument, nullptr, 'd' },
    { "profile", required_argument, nullptr, 'p' },
    { nullptr, 0, nullptr, 0 }
};

void createProfile(const char* profile_name)
{
    auto file_path = ConnectionProfile::createFilePath(profile_name);

    // SECTION - Check if profile already exists
    if (checkPathExist(file_path.c_str()))
    {
        bool isExit(false);
        char choice;
        while(true)
        {
            std::cout << "A profile with this name already exists, if you continue, the data in it will be overwritten. Conitnue creating? [N-no/Y-yes]: ";
            std::cin >> choice;

            if (choice == 'n' || choice == 'N')
                return Logger::getInstance()->logMessage("Profile creation process is stopped");
            else if (choice == 'y' || choice == 'Y')
                isExit = true;

            std::cout << std::endl;

            std::cin.ignore();

            if (isExit)
                break;
        }
    }
    // !SECTION

    // SECTION - Create required variables
    std::string buffer_string; buffer_string.reserve(15); // IPv4 length
    int buffer_integer;

    ConnectionProfile new_profile(profile_name);
    // !SECTION

    // SECTION - Collect values from console
    std::cout << "Fill in the following profile information..." << std::endl;

    std::cout << "Device IPv4 address: ";
    std::cin >> buffer_string;

    if (!new_profile.trySetAddress(buffer_string))
        return Logger::getInstance()->logError("IPv4 format of the device address is not respected ");

    std::cout << "Device port: ";
    std::cin >> buffer_integer;

    if (std::cin.fail() || !new_profile.trySetPort(buffer_integer))
        return Logger::getInstance()->logError("An incorrect value was entered for the port value when creating the profile");

    std::cout << "Profile details: ";
    std::cin.ignore();
    std::getline(std::cin, buffer_string);

    if (!new_profile.trySetDetails(buffer_string))
        return Logger::getInstance()->logError("An incorrect value was entered for the details value when creating the profile");

    std::cout << "Connection buffer size: ";
    std::cin >> buffer_integer;

    if (std::cin.fail() || !new_profile.trySetBufferSize(buffer_integer))
        return Logger::getInstance()->logError("An incorrect value was entered for the buffer size value when creating the profile");
    // !SECTION

    new_profile.save();

    std::cout << "Following profile was created: " << std::endl;
    new_profile.print();
}

void removeProfile(const char* profile_name)
{
    std::string file_path = ConnectionProfile::dir_path;
    file_path += "/";
    file_path += profile_name;
    file_path += ".json";

    if (checkPathExist(file_path.c_str()) && removeFile(file_path.c_str()))
        Logger::getInstance()->logSuccess("Successfully deleted profile on path " + std::string(profile_name));
    else
        Logger::getInstance()->logError("Failed to delete the profile on path " + std::string(profile_name) + " due to its unavailability or an error in operation");
}

void displayProfile(const char* profile_name)
{
    std::vector<std::string> profiles_paths;

    if (strcmp(profile_name, "all") == 0)
        profiles_paths = getFilePaths(ConnectionProfile::dir_path);
    else
        profiles_paths.push_back(profile_name);

    if (profiles_paths.empty())
    {
        std::cout << "No saved profiles are available" << std::endl;
        return;
    }

    for (auto& path : profiles_paths)
    {
        auto current = ConnectionProfile::readFile(path);

        if (current != nullptr)
            current->print();

        std::cout << std::endl;
    }
}

std::shared_ptr<ConnectionProfile> collectProfile(const std::string& profile_name)
{
    std::string profile_path = ConnectionProfile::createFilePath(profile_name);

    if (!checkPathExist(profile_path.c_str()))
    {
        Logger::getInstance()->logError("Failed to locate the requested profile on the intended path " + profile_path);
        return nullptr;
    }

    return ConnectionProfile::readFile(profile_path);
}

std::shared_ptr<ConnectionProfile> parseConnectionSettings(const char* connection_settings)
{
    const std::string params = connection_settings; // format is "address:port@buffer_size"

    std::size_t colon_inx = params.find(':');
    std::size_t ad_inx = params.find('@');

    if (colon_inx == std::string::npos || ad_inx == std::string::npos)
    {
        Logger::getInstance()->logError("Unable to process connection settings due to format mismatch");
        return nullptr;
    }

    auto profile = std::make_shared<ConnectionProfile>("TEMP");

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

int main(int argc, char** argv)
{
    std::unique_ptr<WebClient> web_client;
    std::unique_ptr<WebServer> web_server;
    std::shared_ptr<ConnectionProfile> profile;

    std::string buffer;

    bool isExecuteCalled(false), isDestinationSet(false), isProfileSet(false);

    int option, optinx;
    while ((option = getopt_long(argc, argv, "e:d:c:r:s:p:vh", longOptions, &optinx)) != -1)
    {
        switch (option)
        {
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

        case 'p':
            buffer = optarg; // Saving profile name
            profile = collectProfile(buffer);

            if (profile != nullptr)
            {
                web_client = std::make_unique<WebClient>(*profile);
                isProfileSet = true;
                Logger::getInstance()->logSuccess("Connection profile with name " + buffer + " is successfully obtained");
            }
            else
            {
                Logger::getInstance()->logError("Unable to continue profile assignment due to file read error");
            }
            break;

        case 'd':
            profile = parseConnectionSettings(optarg);

            if (profile != nullptr)
            {
                web_client = std::make_unique<WebClient>(*profile);
                isDestinationSet = true;
                Logger::getInstance()->logSuccess("Settings string has been successfully processed and a temporary connection profile has been created");
            }
            else
            {
                Logger::getInstance()->logError("Unable to continue profile creation due to arguments read error");
            }
            break;

        default:
            Logger::getInstance()->logError("Unrecognized argument received");
            return 1;
        }
    }

    return 0;
}
