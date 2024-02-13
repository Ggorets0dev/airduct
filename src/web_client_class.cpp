#include "web_client_class.hpp"

WebClient::WebClient(const char* ip, int port, int buffer_size) : ip_(ip), port_(port), buffer_size_(buffer_size) {}

bool WebClient::tryConnect(int& client, sockaddr_in& server_address) const
{
    bool status = connect(client,
                          reinterpret_cast<const sockaddr*>(&server_address),
                          sizeof(server_address)) == 0;

    return status;
}

void WebClient::testNet()
{
    int client;

    sockaddr_in server_address;

    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0)
    {
        std::cerr << "CLIENT ERROR: Failed to create a socket" << std::endl;
        exit(1);
    }

    server_address.sin_port = htons(this->port_);
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, this->ip_, &server_address.sin_addr);
    server_address.sin_addr.s_addr = htons(INADDR_ANY);

    std::cout << "Client socket created" << std::endl;

    if (!tryConnect(client, server_address))
    {
        g_main_logger.logError("Failed to establish a connection to the target device on port " + std::to_string(port_) +
                               " and address " + std::string(ip_));
        exit(1);
    }

    std::cout << "Waiting for server confirmation..." << std::endl;

    char buffer[this->buffer_size_];
    recv(client, buffer, this->buffer_size_, 0);

    std::cout << "=> Connection established" << std::endl;

    while (true)
    {
        std::cout << "Client: ";
        std::cin.getline(buffer, this->buffer_size_);

        send(client, buffer, this->buffer_size_, 0);

        std::cout << "Server: ";
        recv(client, buffer, this->buffer_size_, 0);
        std::cout << buffer << std::endl;
    }

    close(client);
}
