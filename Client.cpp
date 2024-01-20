#include "Client.hpp"
#include <string>

Client::Client(int fd)
{
    socket = fd;
    passed = false;
    quit = false;
}

Client::~Client()
{

}

std::string Client::get_nickname() const
{
    return nickname;
}

std::string Client::get_username() const
{
    return username;
}

std::string Client::get_hostname() const
{
    return hostname;
}

int Client::get_socket() const
{
    return socket;
}

void Client::set_passed()
{
    passed = true;
}

void Client::set_socket(int sock)
{
    socket = sock;
}

void Client::set_username(std::string username)
{
    this->username = username;
}

void Client::set_nickname(std::string nick)
{
	nickname = nick;
}

void Client::welcome()
{
    if (has_registered())
   {
       reply(RPL_WELCOME(get_nickname()));
       std::cout << "Registered: " << get_prefix() << std::endl;
   }
}

void Client::set_hostname(std::string hostname)
{
    this->hostname = hostname;
}


bool Client::has_registered() const
{
    if (username != "" && nickname != "" && passed)
        return 1;
    return 0;
}
std::string Client::get_prefix() const
{
    return nickname + (username.empty() ? "" : "!" + username) + 
            (hostname.empty() ? "" : "@" + hostname);
}

void Client::reply(std::string reply)
{
	sending(":" + get_prefix() + " " + reply);
}

void Client::sending(std::string message)
{
    std::string to_send = message + "\r\n";
    send(socket, to_send.c_str(), to_send.length(), 0);
}

bool Client::has_passed() const
{
    return passed;
}


