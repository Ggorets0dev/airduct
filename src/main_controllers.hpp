#ifndef MAIN_CONTROLLERS_H
#define MAIN_CONTROLLERS_H

#include <cstring>
#include <string>
#include <iostream>
#include "connection_profile_class.hpp"
#include "client_profile_class.hpp"
#include "server_profile_class.hpp"

// SECTION - Auxiliary functions for processing command line arguments
/// \brief Display available commands and other argument information
void printHelp();

/// \brief Create a profile depending on the type to be fed
void createProfile(const char* type);

/// \brief Delete one or more profiles
void removeProfile(const char* profile_name);

/// \brief Display one or more profiles
void displayProfile(const char* profile_name);

/// \brief Get a profile from the file where it was previously saved after creation
std::shared_ptr<ConnectionProfile> collectProfile(const std::string& profile_name);

/// \brief Process the lines with connection settings and try to create a profile from them
std::shared_ptr<ConnectionProfile> parseConnectionSettings(const std::string& params);
// !SECTION

#endif // MAIN_CONTROLLERS_H
