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
    { "create-profile", required_argument, nullptr, 'c' },
    { "display-profile", required_argument, nullptr, 'd' },
    { nullptr, 0, nullptr, 0 }
};

void createConnectionProfile(char* profile_name)
{
    // SECTION - Create required variables
    std::string address; address.reserve(15); // IPv4 length
    std::string details;
    int port;
    int buffer_size;

    ConnectionProfile new_profile(profile_name);
    // !SECTION

    // SECTION - Collect values from console
    std::cout << "Fill in the following profile information..." << std::endl;

    std::cout << "Device IPv4 address: ";
    std::cin >> address;

    if (!new_profile.trySetAddress(address))
        return Logger::getInstance()->logError("IPv4 format of the device address is not respected ");

    std::cout << "Device port: ";
    std::cin >> port;

    if (std::cin.fail() || !new_profile.trySetPort(port))
        return Logger::getInstance()->logError("An incorrect value was entered for the port value when creating the profile");

    std::cout << "Profile details: ";
    std::cin.ignore();
    std::getline(std::cin, details);

    if (!new_profile.trySetDetails(details))
        return Logger::getInstance()->logError("An incorrect value was entered for the details value when creating the profile");

    std::cout << "Connection buffer size: ";
    std::cin >> buffer_size;

    if (std::cin.fail() || !new_profile.trySetBufferSize(buffer_size))
        return Logger::getInstance()->logError("An incorrect value was entered for the buffer size value when creating the profile");
    // !SECTION

    const std::string kFilename = std::string(profile_name) + ".json";

    new_profile.save(kFilename);

    std::cout << "Following profile was created: " << std::endl;
    new_profile.print();
}

void displayProfile(char* profile_name)
{
    std::vector<std::string> profiles_filepaths;

    if (strcmp(profile_name, "all") == 0)
        profiles_filepaths = getFilePaths(ConnectionProfile::dir_path);
    else
        profiles_filepaths.push_back(profile_name);

    if (profiles_filepaths.empty())
    {
        std::cout << "No saved profiles are available" << std::endl;
        return;
    }

    for (auto& path : profiles_filepaths)
    {
        auto current = ConnectionProfile::readFile(path);

        if (current != nullptr)
            current->print();

        else
            std::cout << "Failed to open file " << path << std::endl;

        std::cout << std::endl;
    }
}

void printProjectInformation()
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
    while ((option = getopt_long(argc, argv, "d:c:sv", longOptions, &optinx)) != -1)
    {
        switch (option)
        {
        case 'v':
            printProjectInformation();
            break;

        case 's':
            server.testNet();
            break;

        case 'd':
            displayProfile(optarg);
            break;

        case 'c':
            createConnectionProfile(optarg);
            break;

        default:
            std::cerr << "Unknown option: " << static_cast<char>(option) << std::endl;
            return 1;
        }
    }

    return 0;
}
