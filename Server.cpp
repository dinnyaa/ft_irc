#include "Server.hpp"

void throw_error(std::string message, int code)
{
    std::cerr << message << std::endl;
    exit(code);
}

void send_connect_message(int socket, char *addr, short port)
{
    std::cout << "New unregistered user, ";
    std::cout << "socket fd: " << socket
    <<", ip: " << addr
    << ", port: " << port  << "." << std::endl;
}

void send_disconnect_message(char *addr, short port)
{
    std::cout << "Host disconnected";
    std::cout << ", ip: " << addr << ", port: " << port << std::endl;
}

Server::Server(char* port, char* password)
{
    this->port = atoi(port);
    this->server_fd = create_socket();
    this->password = password;
    this->commands = init_commands();
}

void Server::check()
{
    std::cout << "new activity!!!\n";
    std::cout << "Clients found:  " << clients.size() << std::endl;
    for (client_iter it = clients.begin(); it != clients.end(); ++it)
        std::cout <<"Socket = "<< it->get_socket() << "\tnick = "<<it->get_nickname() << "\tuser = " << it->get_username() << std::endl;
    std::cout << "______________________________________________\n\n";

    std::cout << "Channels found: " << channels.size() << std::endl;
    for (channel_it it = channels.begin(); it != channels.end(); ++it)
    {
        Channel * ch = it->second;
        std::cout << "=======================================\n";
        std::cout << "Channel name = " << it->first << "\tpass = " << ch->get_password() << "\n";
        for (size_t i = 0; i < ch->members.size(); ++i)
            std::cout <<"member " <<i + 1 << ") nick = "<<ch->members[i]->get_nickname() << std::endl;
        for (size_t i = 0; i < ch->operators.size(); ++i)
            std::cout <<"operator " <<i + 1 << ") nick = "<<ch->operators[i]->get_nickname() << std::endl;
        std::cout << "=======================================\n\n";
    }
    std::cout <<std::endl;
}

int Server::create_socket()
{                                    
    int master_socket = socket(AF_INET, SOCK_STREAM, 0);
    int opt = true;
    if (master_socket == 0)
        throw_error("Can't open a socket! Quitting", 1);

    if(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
        throw_error("Setsockopt error", 2);

    if (fcntl(master_socket, F_SETFL, O_NONBLOCK))
        throw_error("Can't set socket non blocking! Quitting", 3);

    sockaddr_in  address;
    bzero((char*) &address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(master_socket, (sockaddr *) &address, sizeof(address)) < 0)
        throw_error("Can't bind server address to socket! Quitting", 4);

    if (listen(master_socket, SOMAXCONN) < 0)
        throw_error("Can't listen to clients! Quitting", 5);

    return master_socket;
}

Commands Server::init_commands()
{
    Commands tmp;

    tmp["PASS"] = new Pass(this);
    tmp["NICK"] = new Nick(this);
    tmp["USER"] = new User(this);
    tmp["JOIN"] = new Join(this);
    tmp["CAP"] = new Cap(this);
    tmp["KICK"] = new Kick(this);
    tmp["PRIVMSG"] = new Privmsg(this);
    tmp["NOTICE"] = new Notice(this);
    tmp["PART"] = new Part(this);
    tmp["QUIT"] = new Quit(this);
    tmp["PING"] = new Ping(this);
    tmp["PONG"] = new Pong(this);
    tmp["MODE"] = new Mode(this);

    return tmp;
}

int Server::load_client_sockets()
{
    FD_ZERO(&readfds);
	FD_SET(server_fd, &readfds);
	int max_sd = server_fd;
    int sd;
    for (client_iter it = clients.begin(); it != clients.end(); ++it)
    {
        sd = it->get_socket();		
		if(sd > 0)
			FD_SET(sd , &readfds);	
		if(sd > max_sd)
			max_sd = sd;
    }
    return max_sd;
}

void Server::connect_clients()
{
    timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 300000;
    int max_sd = load_client_sockets();
    int activity = select( max_sd + 1, &readfds, NULL, NULL , &tv);
	if ((activity < 0) && (errno!=EINTR))
		std::cerr << "Select error\n";
    int addrlen = sizeof(address);
    bzero(&address, addrlen);
  
    if (FD_ISSET(server_fd, &readfds))
	{
        int new_socket = accept(server_fd, (sockaddr*)&address, (socklen_t*)&addrlen);
		if (new_socket  < 0)
            throw_error("Can't accept clients", 7);
        if (fcntl(new_socket, F_SETFL, O_NONBLOCK))
            throw_error("Can't set socket non blocking! Quitting", 3);
        send_connect_message(new_socket, inet_ntoa(address.sin_addr), ntohs(port));
		clients.push_back(Client(new_socket));
	}


}


void Server::send_to_exec(char *buffer, Client& client)
{
    if (buffer[0] == '\n')
        return;
    std::vector<std::string> cmds = ft_split(std::string(buffer), "\r\n");

    for (size_t i = 0; i < cmds.size(); ++i)
    {
        if (cmds[i] == "")
            return ;
        Parser parser;
        Name name;
        Arguments args;
        parser(cmds[i]);
        if (!parser.is_valid())
        {
            client.reply(ERR_UNKNOWNCOMMAND(client.get_nickname(), name));
            return ;
        }
        parser.get_command(name, args);
        std::cout << "CMD = " << name << std::endl;
        std::cout << "ARGS = ";
        for (size_t i = 0; i < args.size(); i++)
        {
            std::cout << "["<<args[i] << "]"<< " ";
        }
        std::cout << "\n";
        commands[name]->execute(client, args);
    }
}

std::string Server::get_password() const
{
    return password;
}

int Server::get_server_fd() const
{
    return(server_fd);
}

void Server::handle_IO()
{
    char buffer[1025];
    int valread; 
    int addrlen = sizeof(address);
    for (client_iter it = clients.begin(); it != clients.end(); ++it)
    {
        int sd = it->get_socket(); 
        if (FD_ISSET(sd, &readfds))
        {   //client has disconnected              
            if ((valread = recv(sd, buffer, 1024, 0)) == 0)                         
            {
                getsockname(sd, (sockaddr*)&address, (socklen_t*)&addrlen);
                send_disconnect_message(inet_ntoa(address.sin_addr), ntohs(port));
                close(sd);
                client_iter tmp = it;
                it--;
                remove_from_channels(&(*tmp));
                clients.erase(tmp);
            }   
            else //client sent a message
            {
                buffer[valread] = '\0';
                send_to_exec(buffer, *it);
                if (it->quit)
                {
                    getsockname(sd, (sockaddr*)&address, (socklen_t*)&addrlen);
                    send_disconnect_message(inet_ntoa(address.sin_addr), ntohs(port));
                    close(sd);
                    client_iter tmp = it;
                    it--;
                    remove_from_channels(&(*tmp));
                    clients.erase(tmp);
                }
                    
            }
            check();
        }
    }
}

bool Server::is_client(std::string nick)
{
    for (client_iter it = clients.begin(); it != clients.end(); ++it)
        if (nick == it->get_nickname())
        return 1;
    return 0;
}

void Server::remove_from_channels(Client* client)
{
    channel_it it = channels.begin();
    while (it != channels.end())
    {
        Channel *channel = it->second;
        channel->remove_member(client);
        if (channel->members.size() == 0)
        {
            delete channel;
            it = channels.erase(it);
        }
        else
            ++it;
    }
}

Server::~Server()
{
    for(command_iter it = commands.begin(); it != commands.end(); ++it)
       delete it->second;
    for (channel_it it = channels.begin(); it != channels.end(); ++it)
        delete it->second;
}

Client *Server::find_client(std::string nickname)
{
    client_iter it = clients.begin();
    for (; it != clients.end(); ++it)
    {
        if (it->get_nickname() == nickname)
            return &(*it);
    }
    return NULL;
}

void Server::add_channel(std::string name, Channel *channel)
{
    channels.insert(std::make_pair(name, channel));
}

void Server::remove_channel(std::string name)
{
    channels.erase(name);
}

Channel* Server::find_channel(std::string name)
{
    channel_it it = channels.find(name);
    if (it == channels.end())
        return (NULL);
    return (it->second);
}

void Server::notify_clients(std::string message)
{
    for (client_iter it = clients.begin(); it != clients.end(); ++it)
        it->sending(message);
}
