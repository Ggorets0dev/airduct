#ifndef CLIENTPROFILE_HPP
#define CLIENTPROFILE_HPP

#include <string>
#include <memory>
#include "connection_profile_class.hpp"

class ClientProfile final : public ConnectionProfile
{
public:
    static const char* dir_path;

    static std::shared_ptr<ClientProfile> createFromFile(const std::string& path);
    static std::string getFilePath(const std::string& profile_name);

    ClientProfile(const std::string& name) : ConnectionProfile(name) {};

    std::string getAddress() const  { return address_; };
    bool trySetAddress(const std::string& text);

    void fillFromCin() override;
    void save() const override;
    void print() const override;

private:
    std::string address_;
};

#endif // CLIENTPROFILE_HPP
