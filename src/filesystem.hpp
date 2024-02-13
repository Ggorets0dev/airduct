#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <cstring>
#include <vector>
#include <dirent.h>
#include <string>
#include <algorithm>
#include "logger_class.hpp"

std::vector<std::string> getFileNames(const std::string &path);
std::vector<std::string> getFilePaths(const std::string &path);

#endif // FILESYSTEM_H
