#pragma once
#include <vector>
#include <string>
#include <cctype>
#include <iostream>
#include "../Replies.hpp"

class Server;
class Client;
class Channel;

typedef std::string                   Name;
typedef std::vector<std::string>      Arguments;

class Command
{
public:
    Command(Server *server);
    virtual ~Command();
    virtual void execute(Client&, Arguments) = 0;
protected:
    Server *serv;
};

class Cap : public Command 
{
public:
    Cap(Server *server);
    ~Cap();
    void execute(Client&, Arguments);
};

class Pass : public Command 
{
public:
    Pass(Server *server);
    ~Pass();
    void execute(Client&, Arguments);
};

class Nick : public Command 
{
public:
    Nick(Server *server);
    ~Nick();
    bool is_valid(std::string nick);
    void execute(Client&, Arguments);
};

class User : public Command 
{
public:
    User(Server *server);
    ~User();
    void execute(Client&, Arguments);
};

class Join : public Command 
{
    void join_impl(std::string &channel, std::string &key, Client& client);
public:
    Join(Server *server);
    ~Join();
    void execute(Client&, Arguments);
};

class Kick : public Command 
{
public:
    Kick(Server *server);
    ~Kick();
    void execute(Client&, Arguments);
};

class Privmsg : public Command 
{
public:
    Privmsg(Server *server);
    ~Privmsg();
    void execute(Client&, Arguments);
};

class Notice : public Command 
{
public:
    Notice(Server *server);
    ~Notice();
    void execute(Client&, Arguments);
};

class Part : public Command 
{
public:
    Part(Server *server);
    ~Part();
    void execute(Client&, Arguments);
};

class Quit : public Command 
{
public:
    Quit(Server *server);
    ~Quit();
    void execute(Client&, Arguments);
};

class Ping : public Command 
{
public:
    Ping(Server *server);
    ~Ping();
    void execute(Client&, Arguments);
};

class Pong : public Command 
{
public:
    Pong(Server *server);
    ~Pong();
    void execute(Client&, Arguments);
};

class Mode : public Command 
{
public:
    Mode(Server *server);
    ~Mode();
    void execute(Client&, Arguments);
private:
    void exec_k(Client& client, Channel *channel, char sign, Arguments args);
    void exec_l(Client& client, Channel *channel, char sign, Arguments args);
    void exec_o(Client& client, Channel *channel, char sign, Arguments args);
};

#include "../Client.hpp"
#include "../Server.hpp"
#include "../Channel.hpp"