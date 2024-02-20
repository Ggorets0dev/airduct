#include "web_client_class.hpp"

WebClient::WebClient(const char* ip, int port, int buffer_size) : WebDevice(port, buffer_size)
{
    ip_ = ip;
}

WebClient::WebClient(const ClientProfile& profile) : WebDevice(profile.getPort(), profile.getBufferSize())
{
    ip_ = profile.getAddress().c_str();
}

bool WebClient::tryConnect(int client) const
{
    bool status = connect(client,
                          reinterpret_cast<const sockaddr*>(&target_address_),
                          sizeof(target_address_)) == 0;

    return status;
}

std::string WebClient::getMessage()
{
    std::string exception_text;
    int client;

    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0)
        throw std::runtime_error("Failed to create socket without binding data");

    Logger::getInstance()->logSuccess("A socket object was successfully created without binding data to it");

    configureSocket();

    if (!tryConnect(client))
        throw std::runtime_error("Failed to establish a connection to the target device");

    Logger::getInstance()->logSuccess("Сonnection to the target device has been successfully established");
    Logger::getInstance()->logMessage("Data is expected from the target device...");

    char buffer[this->buffer_size_];
    recv(client, buffer, this->buffer_size_, 0);

    close(client);

    return std::string(buffer);
}


void WebClient::configureSocket()
{
    target_address_.sin_port = htons(port_);
    target_address_.sin_family = AF_INET;
    inet_pton(AF_INET, ip_, &target_address_.sin_addr);
    target_address_.sin_addr.s_addr = htons(INADDR_ANY);
}
