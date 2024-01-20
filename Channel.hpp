#pragma once
#include "Server.hpp"

class Channel
{
    public:
    
        Channel();
        ~Channel();

        void set_password(std::string m_password);
        bool is_member(Client* client);
        void remove_member(Client* client);
        void print_members(Client *client);
        void print_members();
        void add_user(Client* client);
        bool is_operator(Client* client);
        void remove_operator(Client* client);
        void add_operator(Client* client);
        void sending(Client* client, std::string* message, const std::string& command, Client* op, std::string* ch_name);

        std::string		get_name();
	    std::string		get_password();

    private:

        std::string name;
        std::string	password;
		std::string	topic;

        // banned users
        // invites


        // members
        // operators/author topics
public:
        std::vector<Client*> members;
        std::vector<Client*> operators;
};

bool is_channel_name(std::string channelName);
std::vector<std::string> ft_split(const std::string& str, const std::string& delimiter);
