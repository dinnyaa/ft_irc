#include "Command.hpp"
#include "../Channel.hpp"

Join::Join(Server *server) : Command(server) {}

Join::~Join() {}

void Join::join_impl(std::string &channel_name, std::string &key, Client& client)
{
    if (!is_channel_name(channel_name))
    {
        client.reply(ERR_BADCHANMASK(client.get_nickname(), channel_name));
        return ;
    }
    Channel* channel = serv->find_channel(channel_name);
    if(channel)
    { 
        if(channel->get_password() != key)
        {
            client.reply(ERR_PASSWDMISMATCH(channel_name));
            return ;
        }
        if (channel->is_member(&client))
            client.reply("You are already in that channel.");
        else
        {
            channel->add_user(&client);
            channel->sending(&client, NULL, "JOIN", NULL, &channel_name);
            channel->print_members(&client);
        }
    }
    else
    {
        Channel* new_channel = new Channel();
        serv->add_channel(channel_name, new_channel);
        new_channel->set_password(key);
        new_channel->add_user(&client);
        new_channel->sending(&client, NULL, "JOIN", NULL, &channel_name);
        new_channel->add_operator(&client);
        new_channel->print_members(&client);
        new_channel->print_members(&client);

    }
}

void Join::execute(Client& client, Arguments args)
{
    //channels
    if(!client.has_registered())
    {
        client.reply(ERR_NOTREGISTERED(client.get_nickname()));
        return ;
    }
    if(args.size() < 1)
    {
        client.reply(ERR_NEEDMOREPARAMS(client.get_nickname(), "JOIN"));
        return;
    }
    Arguments join_channels = ft_split(args[0], ",");
    std::string empty = "";
    if(args.size() == 1)
    {
        for (size_t j = 0; j < join_channels.size(); j++)
            join_impl(join_channels[j], empty, client);
        return ;
    }
    Arguments join_keys = ft_split(args[1], ",");
   
    if(join_channels.size() < join_keys.size())
    {
        client.reply(ERR_NEEDMOREPARAMS(client.get_nickname(), "JOIN"));
        return ;
    }

    for (size_t j = 0; j < join_channels.size(); j++)
    {
        if ( j  < join_keys.size())
            join_impl(join_channels[j], join_keys[j], client);
        else
            join_impl(join_channels[j], empty, client);
    }
}


