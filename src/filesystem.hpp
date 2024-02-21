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

///
/// \brief Performing operations with the distribution file system
///
namespace filesystem
{
    ///
    /// \brief Getting all file names in the specified directory
    /// \param path - Directory path
    /// \return Array of file names
    ///
    std::vector<std::string> getFileNames(const std::string &path);

    ///
    /// \brief Getting all file paths in the specified directory
    /// \param path - Directory path
    /// \return Array of file paths
    ///
    std::vector<std::string> getFilePaths(const std::string &path);

    ///
    /// \brief Check if specified dir or file exist
    /// \param path - Directory of file path
    /// \return Status of existence
    ///
    bool checkPathExist(const char* path);

    ///
    /// \brief Create directory with specified path
    /// \param path - Directory path
    /// \return Status of success
    ///
    bool createDirectory(const char* path);

    ///
    /// \brief Remove file with specified path
    /// \param path - File path
    /// \return Status of success
    ///
    bool removeFile(const char* path);

    ///
    /// \brief Remove file with specified path
    /// \param path - File path
    /// \return Status of success
    ///
    bool removeDirFiles(const char* path);

    ///
    /// \brief Save JSON object with specified path
    /// \param path - File path
    /// \return Status of success
    ///
    void saveJson(const rapidjson::Document& doc, const std::string& path);

    ///
    /// \brief Read JSON object from file with specified path
    /// \param path - File path
    /// \return Status of success
    ///
    std::shared_ptr<rapidjson::Document> readJson(const std::string& path);

    ///
    /// \brief Execute command in system
    /// \param cmd - Command to execute
    /// \return Command output
    ///
    std::string executeCommand(const char* cmd);
}

#endif // FILESYSTEM_H
