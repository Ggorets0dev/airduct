#ifndef MAIN_CONTROLLERS_H
#define MAIN_CONTROLLERS_H

#include <cstring>
#include <string>
#include <iostream>
#include "connection_profile_class.hpp"
#include "client_profile_class.hpp"
#include "server_profile_class.hpp"

// SECTION - Auxiliary functions for processing command line arguments
void printHelp();

void createProfile(const char* type);

void removeProfile(const char* profile_name);

void displayProfile(const char* profile_name);

std::shared_ptr<ConnectionProfile> collectProfile(const std::string& profile_name);

std::shared_ptr<ConnectionProfile> parseConnectionSettings(const std::string& params);
// !SECTION

#endif // MAIN_CONTROLLERS_H
