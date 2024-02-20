#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <cstring>
#include <vector>
#include <dirent.h>
#include <string>
#include <algorithm>
#include <sys/stat.h>
#include <memory>
#include "logger_class.hpp"

#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/writer.h"

std::vector<std::string> getFileNames(const std::string &path);

std::vector<std::string> getFilePaths(const std::string &path);

bool checkPathExist(const char* path);

bool createDirectory(const char* path);

bool removeFile(const char* path);

bool removeDirFiles(const char* dir_path);

void saveJson(const rapidjson::Document& doc, const std::string& path);

std::shared_ptr<rapidjson::Document> readJson(const std::string& path);

std::string executeCommand(const char* cmd);

#endif // FILESYSTEM_H
