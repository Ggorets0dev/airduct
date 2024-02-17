#ifndef WEBDEVICE_HPP
#define WEBDEVICE_HPP

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

class WebDevice
{
public:
    WebDevice(int port, int buffer_size);

protected:
    sockaddr_in target_address_;
    int port_;
    int buffer_size_;

    virtual void configureSocket() = 0;
};

#endif // WEBDEVICE_HPP
