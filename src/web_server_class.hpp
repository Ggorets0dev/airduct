#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "logger_class.hpp"
#include "web_device_class.hpp"
#include "server_profile_class.hpp"

class WebServer final : public WebDevice
{
public:
    WebServer(int port_, int buffer_size_) : WebDevice(port_, buffer_size_) {};
    WebServer(const ServerProfile& profile) : WebDevice(profile.getPort(), profile.getBufferSize()) {};
    void sendMessage(const std::string& text);

protected:
    void configureSocket() override;
};

#endif // SERVER_H
