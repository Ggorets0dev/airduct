#ifndef WEBCLIENTCLASS_HPP
#define WEBCLIENTCLASS_HPP

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "client_profile_class.hpp"
#include "logger_class.hpp"
#include "web_device_class.hpp"

class WebClient final : public WebDevice
{
public:
    WebClient(const char* ip, int port, int buffer_size);
    WebClient(const ClientProfile& profile);

    /// \brief Receive a message from the server by taking the connection settings from the class fields
    std::string getMessage();

private:
    const char* ip_;

    void configureSocket() override;
    bool tryConnect(int client) const;
};

#endif // WEBCLIENTCLASS_HPP
