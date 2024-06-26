#ifndef CLIENTPROFILE_HPP
#define CLIENTPROFILE_HPP

#include <string>
#include <memory>
#include "connection_profile_class.hpp"

#define IPV4_PATTERN "^(25[0-4]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]?)\\.(25[0-4]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]?)\\.(25[0-4]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]?)\\.(25[0-4]|2[0-4][0-8]|1[0-9][0-9]|[1-9][0-9]|[1-9])$"

class ClientProfile final : public ConnectionProfile
{
public:
    static const char* dir_path;

    ///
    /// \brief Create object from its file variation
    /// \param path - File path
    /// \return Created object
    ///
    static std::shared_ptr<ClientProfile> createFromFile(const std::string& path);

    ///
    /// \brief Generate path of profile
    /// \param profile_name - Name of client profile
    /// \return Generated file path
    ///
    static std::string getFilePath(const std::string& profile_name);

    ClientProfile(const std::string& name) : ConnectionProfile(name) {}

    /// \brief Getter for Address field
    std::string getAddress() const  { return address_; }

    /// \brief Setter for Address field
    bool trySetAddress(std::string& text);

    void fillFromString(const std::string& text) override;
    void fillFromCin() override;
    void save() const override;
    void print() const override;

private:
    std::string address_;

    bool convertLocalhost(std::string& address);
};

#endif // CLIENTPROFILE_HPP
