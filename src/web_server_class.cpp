#include "web_server_class.hpp"

void WebServer::sendMessage(const std::string& text)
{
    std::string exception_text;
    int server, client;

    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0)
    {
        exception_text = "Failed to create a socket object before binding data to it";

        Logger::getInstance()->logError(exception_text);
        throw std::runtime_error(exception_text);
    }

    Logger::getInstance()->logSuccess("A socket object was successfully created without binding data to it");

    configureSocket();

    int bind_status = bind(client, reinterpret_cast<sockaddr*>(&target_address_), sizeof(target_address_));

    if (bind_status < 0)
    {
        exception_text = "Failed to assign a created socket in the operating system";

        Logger::getInstance()->logError(exception_text);
        throw std::runtime_error(exception_text);
    }

    Logger::getInstance()->logSuccess("Socket has been successfully installed in the system and is ready for operation");
    Logger::getInstance()->logMessage("A client is expected to connect to this device...");

    socklen_t size = sizeof(target_address_);
    listen(client, 1);
    server = accept(client, reinterpret_cast<sockaddr*>(&target_address_), &size);

    if (server < 0)
    {
        exception_text = "Failed to connect to the client that attempted the connection";

        Logger::getInstance()->logError(exception_text);
        throw std::runtime_error(exception_text);
    }

    char buffer[this->buffer_size_];
    strcpy(buffer, text.c_str());

    send(server, buffer, this->buffer_size_, 0);
    //recv(server, buffer, this->buffer_size_, 0);
}

void WebServer::configureSocket()
{
    target_address_.sin_port = htons(port_);
    target_address_.sin_family = AF_INET;
    target_address_.sin_addr.s_addr = htons(INADDR_ANY);
}
