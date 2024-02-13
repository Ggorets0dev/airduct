#include "web_server_class.hpp"

WebServer::WebServer(int port, int buffer_size) : port_(port), buffer_size_(buffer_size) {}

bool WebServer::isCloseRequest(const char* msg)
{
    for (std::size_t i(0); i < strlen(msg); ++i)
        if (msg[i] == CLOSE_CONNECTION_SYMBOL)
            return true;

    return false;
}

void WebServer::testNet()
{
    int server, client;
    sockaddr_in server_address;

    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0)
    {
        std::cerr << "SERVER ERROR: Socket can't be established" << std::endl;
        exit(1);
    }

    std::cout << "Socket created" << std::endl;

    server_address.sin_port = htons(this->port_);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htons(INADDR_ANY);

    int ret = bind(client, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address));

    if (ret < 0)
    {
        std::cerr << "SERVER ERROR: Failed binding connection" << std::endl;
        exit(1);
    }

    socklen_t size = sizeof(server_address);
    std::cout << "Listening clients..." << std::endl;
    listen(client, 1);

    server = accept(client, reinterpret_cast<sockaddr*>(&server_address), &size);

    if (server < 0)
        std::cout << "Can't accept client" << std::endl;

    char buffer[this->buffer_size_];
    bool is_exit = false;

    while (server > 0)
    {
        strcpy(buffer, "=> Server connected!\n");
        send(server, buffer, this->buffer_size_, 0);
        std::cout << "=> Connected to the client #1" << std::endl;

        std::cout << "Client: ";
        recv(server, buffer, this->buffer_size_, 0);
        std::cout << buffer << std::endl;

        if (isCloseRequest(buffer))
            is_exit = true;

        while (!is_exit)
        {
            std::cout << "Server: ";
            std::cin.getline(buffer, this->buffer_size_);
            send(server, buffer, this->buffer_size_, 0);

            if (isCloseRequest(buffer))
                break;

            std::cout << "Client: ";
            recv(server, buffer, this->buffer_size_, 0);
            std::cout << buffer << std::endl;

            if (isCloseRequest(buffer))
                break;
        }

        std::cout << "Goodbye..." << std::endl;
        is_exit = false;
        exit(1);
    }
}
