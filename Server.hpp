#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <list>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

#include "Client.hpp"
#include "Channel.hpp"
#include "./commands/Command.hpp"

typedef std::string                         Name;
typedef std::vector<std::string>            Arguments;

typedef std::list<Client>                   Clients;
typedef std::map<Name, Command*>            Commands;
typedef std::map<std::string, Channel*>     Channels;

typedef Clients::iterator         		    client_iter;
typedef Commands::iterator                  command_iter;
typedef Channels::iterator  	            channel_it;

#include "Parser.hpp"

class Server
{
public:
    
    Server(char* port, char* password);
    void connect_clients();
    void handle_IO();
    void notify_clients(std::string);
    bool is_client(std::string nick);
    std::string get_password() const;
    int get_server_fd() const;
    ~Server();
    client_iter remove_client(client_iter it);
    Client *find_client(std::string client_info);
    //start
  

   	void remove_channel(std::string name);
    void remove_from_channels(Client* client);
   	void add_channel(std::string name, Channel *channel);
   	Channel* find_channel(std::string name);
    //end
private:
    int create_socket();
    int load_client_sockets();
    Commands init_commands();
    void check();
    void send_to_exec(char *buffer, Client& client);
    
private:
    short		port;
    int 		server_fd;
    Channels    channels;
    Commands	commands;
    std::string password;
    fd_set		readfds;
    Clients		clients;
    sockaddr_in address;
};
