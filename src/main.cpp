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
std::string ConnectionProfile::dir_path = "./profiles";

const option longOptions[] = {
    { "version", no_argument, nullptr, 'v' },
    { "help", no_argument, nullptr, 'v' },
    { "create", required_argument, nullptr, 'c' },
    { "show", required_argument, nullptr, 's' },
    { "execute", required_argument, nullptr, 'e' },
    { "remove", required_argument, nullptr, 'r' },
    { "destination", required_argument, nullptr, 'd' },
    { nullptr, 0, nullptr, 0 }
};

void createProfile(char* profile_name)
{
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

    const std::string kFilename = std::string(profile_name) + ".json";

    new_profile.save(kFilename);

    std::cout << "Following profile was created: " << std::endl;
    new_profile.print();
}

void removeProfile(char* profile_name)
{

}

void displayProfile(char* profile_name)
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

        else
            std::cout << "Failed to open file " << path << std::endl;

        std::cout << std::endl;
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
    WebClient client("127.0.0.1", 5632, 1000);
    WebServer server(5632, 1000);

    int option, optinx;
    while ((option = getopt_long(argc, argv, "e:d:c:r:s:vh", longOptions, &optinx)) != -1)
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

        default:
            Logger::getInstance()->logError("Unrecognized argument received");
            return 1;
        }
    }

    return 0;
}
