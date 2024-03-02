#include <getopt.h>
#include <vector>
#include <functional>
#include "filesystem.hpp"
#include "web_server_class.hpp"
#include "web_client_class.hpp"
#include "logger_class.hpp"
#include "client_profile_class.hpp"
#include "server_profile_class.hpp"
#include "main_controllers.hpp"
#include "project_info.h"
#include "version.h"

Logger* Logger::global_instance = nullptr;
const char* ClientProfile::dir_path = "./client_profiles";
const char* ServerProfile::dir_path = "./server_profiles";

void printSoftwareInformation()
{
    std::cout << "AirDuct v" << g_version << std::endl;
    std::cout << "Developer: " << g_developer << std::endl;
    std::cout << "Repository: " << g_repository << std::endl;
    std::cout << "License: " << g_license << std::endl;
}

const option longOptions[] = {
    { "version", no_argument, nullptr, 'v' },
    { "help", no_argument, nullptr, 'h' },
    { "create", required_argument, nullptr, 'c' },
    { "show", required_argument, nullptr, 's' },
    { "execute", required_argument, nullptr, 'e' },
    { "remove", required_argument, nullptr, 'r' },
    { "destination", required_argument, nullptr, 'd' },
    { "profile", required_argument, nullptr, 'p' },
    { "transmit", no_argument, nullptr, 't' },
    { "obtain", no_argument, nullptr, 'o' },
    { nullptr, 0, nullptr, 0 }
};

int main(int argc, char** argv)
{
    std::shared_ptr<ConnectionProfile> profile;
    const char* command;

    std::string buffer; // NOTE - For different operations in switch-case statement

    bool isExecuteCalled {false};
    ConnectionSettingsSource source {ConnectionSettingsSource::NotSelected};
    NetworkMode mode {NetworkMode::NotSelected};

    int option, optinx;
    while ((option = getopt_long(argc, argv, "e:d:c:r:s:p:vhto", longOptions, &optinx)) != -1)
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
            if (strlen(optarg) != 0)
            {
                command = optarg;
                isExecuteCalled = true;
            }
            else
            {
                Logger::getInstance()->logError("Failed to save a console command that does not meet the requirements");
            }
            break;

        case 't':
            mode = NetworkMode::Server;
            Logger::getInstance()->logMessage("Information sending mode is set");
            break;

        case 'o':
            mode = NetworkMode::Client;
            Logger::getInstance()->logMessage("Information acquisition mode is set");
            break;

        case 'p':
            profile = collectProfile(optarg);

            if (profile != nullptr)
                source = ConnectionSettingsSource::Profile;
            else
                Logger::getInstance()->logError("Unable to continue profile assignment due to file read error");

            break;

        case 'd':
            profile = parseConnectionSettings(optarg);

            if (profile != nullptr)
                source = ConnectionSettingsSource::OneTime;
            else
                Logger::getInstance()->logError("Unable to continue profile creation due to arguments read error");

            break;

        default:
            Logger::getInstance()->logError("Unrecognized argument received");
            return 1;
        }
    }

    auto logModeError = []() { Logger::getInstance()->logError("Required to select whether to receive or send data"); };
    auto logConnectionError = []() { Logger::getInstance()->logError("Required to define the connection settings: select an existing one profile or enter one-time connection settings"); };
    auto logProfileTypeError = []() { Logger::getInstance()->logError("Type of operation does not match the specified profile"); };

    bool isModeChosen {mode != NetworkMode::NotSelected};
    bool isConnectionSourceDefined {source != ConnectionSettingsSource::NotSelected};

    // SECTION - Processing operations
    if (isExecuteCalled && isModeChosen && isConnectionSourceDefined)
    {
        if (mode == NetworkMode::Server)
        {
            auto server_profile = std::dynamic_pointer_cast<ServerProfile>(profile);

            if (server_profile == nullptr)
            {
                logProfileTypeError();
                return 1;
            }

            auto web_server = std::make_unique<WebServer>(*server_profile);

            try
            {
                std::string command_output = filesystem::executeCommand(command);
                web_server->sendMessage(command_output);
                Logger::getInstance()->logSuccess("Message successfully sent to the connected client");
            }
            catch (std::runtime_error& error)
            {
                Logger::getInstance()->logError(error.what());
            }
        }
        else if (mode == NetworkMode::Client)
        {
            auto client_profile = std::dynamic_pointer_cast<ClientProfile>(profile);

            if (client_profile == nullptr)
            {
                logProfileTypeError();
                return 1;
            }

            auto web_client = std::make_unique<WebClient>(*client_profile);

            try
            {
                std::string raw_command = web_client->getMessage();
                Logger::getInstance()->logSuccess("Message successfully recieved from server and processed by the requested command");

                raw_command = "echo \"" + raw_command + "\" | " + std::string(command);

                std::cout << "Command has been processed and has the following output:" << std::endl;
                std::cout << filesystem::executeCommand(raw_command.c_str()) << std::endl;
            }
            catch (std::runtime_error& error)
            {
                Logger::getInstance()->logError(error.what());
            }
        }
    }
    else if (isExecuteCalled && isModeChosen)
    {
        logConnectionError();
    }
    else if (isExecuteCalled && isConnectionSourceDefined)
    {
        logModeError();
    }
    else if (isExecuteCalled)
    {
        logModeError();
        logConnectionError();
    }
    // !SECTION

    return 0;
}
