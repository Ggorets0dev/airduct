#ifndef WEBCLIENTCLASS_HPP
#define WEBCLIENTCLASS_HPP

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

class WebClient
{
public:
    WebClient(const char* ip, int port, int buffer_size);
    void testNet();

private:
    const char* ip_;
    int port_;
    int buffer_size_;
};

#endif // WEBCLIENTCLASS_HPP
