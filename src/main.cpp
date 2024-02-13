#include <iostream>
#include <getopt.h>
#include "project_info.h"
#include "version.h"
#include "web_server_class.hpp"
#include "web_client_class.hpp"
#include "rapidjson/document.h"

const option longOptions[] = {
    { "version", no_argument, nullptr, 'v' },
    { nullptr, 0, nullptr, 0 }
};

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
    while ((option = getopt_long(argc, argv, "scv", longOptions, &optinx)) != -1)
    {
        switch (option)
        {
        case 'v':
            printProjectInformation();
            break;

        case 's':
            server.testNet();
            break;

        case 'c':
            client.testNet();
            break;

        default:
            std::cerr << "Unknown option: " << static_cast<char>(option) << std::endl;
            return 1;
        }
    }

    return 0;
}
