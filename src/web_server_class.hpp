#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>

class WebServer
{
public:
    WebServer(int port, int buffer_size);
    void testNet();

private:
    static const int CLOSE_CONNECTION_SYMBOL = static_cast<int>('#');

    int buffer_size_;
    int port_;

    bool isCloseRequest(const char* msg);
};

#endif // SERVER_H
