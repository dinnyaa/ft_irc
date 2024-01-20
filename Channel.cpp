#include "Server.hpp"

Channel::Channel()  {}

Channel::~Channel() {}

void Channel::add_user(Client* client)
{
    members.push_back(client);
}

void Channel::print_members()
{
    for(std::vector<Client*>::iterator it = members.begin(); it != members.end(); ++it)
    {
        print_members(*it);
    }
}
void Channel::print_members(Client *client)
{
    for(std::vector<Client*>::iterator it = members.begin(); it != members.end(); ++it)
    {
        if (is_operator(*it))
            client->sending(RPL_NAMREPLY(client->get_nickname(), name, "@", (*it)->get_nickname()));
        else
            client->sending(RPL_NAMREPLY(client->get_nickname(), name, "+", (*it)->get_nickname()));
    }
    client->sending(RPL_ENDOFNAMES(client->get_nickname(), name));
}

bool Channel::is_member(Client* client)
{
    size_t i = 0;

    while (i < members.size())
    {
        if (members[i] == client)
            return true;
        i++;
    }
    return false;
}

void Channel::remove_member(Client* client)
{
    if (is_operator(client) == true)
        remove_operator(client);
    for(std::vector<Client*>::iterator it = members.begin(); it != members.end(); ++it)
    {
        if (client == (*it))
        {
            members.erase(it);
            break ;
        }
    }
    if (operators.size() == 0 && members.size() > 0)
        add_operator((members.front()));
}

void Channel::add_operator(Client* client)
{
    std::string message = "is a new admin";
    client->sending(RPL_MSG(client->get_prefix(), "", name, "you are the new admin"));
    this->sending(client, &message, "", NULL, &name);
    operators.push_back(client);
}

bool Channel::is_operator(Client* client)
{
    size_t i = 0;

    while (i < operators.size())
    {
        if (client == operators[i])
            return true;
        i++;
    }
    return false;
}

void Channel::remove_operator(Client* client)
{
    for(std::vector<Client*>::iterator it = operators.begin(); it != operators.end(); ++it)
    {
        std::string message = "is not admin anymore";
        if (client == (*it))
        {
            client->sending(RPL_MSG(client->get_prefix(), "", name, "you are not admin anymore"));
            this->sending(client, &message, "", NULL, &name);
            operators.erase(it);
            break ;
        }
    }
}

std::string Channel::get_name()
{
    return name;
}

std::string Channel::get_password()
{
    return password;
}


void Channel::set_password(std::string m_password)
{
    password = m_password;
}



void Channel::sending(Client* client, std::string *message, const std::string& command, Client* op, std::string* ch_name)
{
    for(std::vector<Client*>::iterator it = members.begin(); it != members.end(); ++it)
    {
        if (command == "PRIVMSG" || command == "NOTICE")
        {
            if (*it != client)
                (*it)->sending(RPL_MSG(client->get_prefix(), command, *ch_name, *message));
        }
        else if (command == "KICK")
            (*it)->sending(RPL_KICK(op->get_prefix(), *ch_name, client->get_nickname(), *message));
        else if (command == "PART")
            (*it)->sending(RPL_PART(client->get_prefix(), *ch_name));
        else if (command == "JOIN")
            (*it)->sending(RPL_JOIN(client->get_prefix(), *ch_name));
        else if (command == "")
        {
            if (*it != client)
                (*it)->sending(RPL_MSG(client->get_prefix(), command, *ch_name, *message));
        }

    }
}

bool is_channel_name(std::string channelName)
{
    if (channelName.empty())
    {
        return false;
    }
    if (channelName[0] != '#' && channelName[0] != '&') {

        return false;
    }
    
    return true;
}

std::vector<std::string> ft_split(const std::string& str, const std::string& delimiter)
{
    // Returns first token
    char *token = strtok(const_cast<char*>(str.c_str()), const_cast<char*>(delimiter.c_str()));
    std::vector<std::string> substrings;
   
    // Keep printing tokens while one of the
    // delimiters present in str[].
    while (token != NULL)
    {
        substrings.push_back(token);
        token = strtok(NULL, const_cast<char*>(delimiter.c_str()));
    }
    return substrings;
}
