#pragma once
#include <string>
#include <sys/socket.h>
#include <iostream>
#include "Replies.hpp"

class Client
{
public:
    bool quit;
    Client(int fd);
    ~Client();

    int get_socket() const;
    std::string get_prefix() const;
    std::string get_username() const;
    std::string get_nickname() const;
    std::string get_hostname() const;


    void set_socket(int sock);
    void set_username(std::string username);
    void set_nickname(std::string nickname);
    void set_hostname(std::string hostkname);
    void set_passed();

    bool has_registered() const;
    bool has_passed() const;
    void reply(std::string message);
    void sending(std::string message);
    void welcome();


private:
    int         socket;
    std::string username;
    std::string nickname;
    std::string hostname;
    bool        passed;
};