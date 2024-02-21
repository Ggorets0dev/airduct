#ifndef SERVERPROFILE_HPP
#define SERVERPROFILE_HPP

#include <string>
#include <memory>
#include "filesystem.hpp"
#include "connection_profile_class.hpp"

class ServerProfile : public ConnectionProfile
{
public:
    static const char* dir_path;

    ///
    /// \brief Create object from its file variation
    /// \param path - File path
    /// \return Created object
    ///
    static std::shared_ptr<ServerProfile> createFromFile(const std::string& path);

    ///
    /// \brief Generate path of profile
    /// \param profile_name - Name of client profile
    /// \return Generated file path
    ///
    static std::string getFilePath(const std::string& profile_name);

    ServerProfile(const std::string& name) : ConnectionProfile(name) {};

    void save() const override;
    void print() const override;

};

#endif // SERVERPROFILE_HPP
