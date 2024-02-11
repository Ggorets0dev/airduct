#include <iostream>
#include <unistd.h>
#include "project_info.h"
#include "version.h"
#include "rapidjson/document.h"

void printProjectInformation()
{
    std::cout << "AirDuct v" << g_version << std::endl;
    std::cout << "Developer: " << g_developer << std::endl;
    std::cout << "Repository: " << g_repository << std::endl;
    std::cout << "License: " << g_license << std::endl;
}

int main(int argc, char** argv)
{
    int option;
    while ((option = getopt(argc, argv, "v")) != -1)
    {
        switch (option)
        {
        case 'v':
            printProjectInformation();
            break;

        default:
            std::cerr << "Unknown option: " << static_cast<char>(option) << std::endl;
            return 1;
        }
    }

    return 0;
}
