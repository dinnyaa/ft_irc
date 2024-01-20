#include "Command.hpp"

Part::Part(Server *server) : Command(server) {}

Part::~Part() {}

void Part::execute(Client& client, Arguments args)
{
   
    if(!client.has_registered())
    {
        client.reply(ERR_NOTREGISTERED(client.get_nickname()));
        return;
    }
    if(args.size() != 1)
    {
        client.reply(ERR_NEEDMOREPARAMS(client.get_nickname(), "PART"));
        return;
    }
         
    std::vector<std::string> channel_names = ft_split(args[0], ",");

    for(size_t i = 0; i < channel_names.size(); ++i)
    {
        Channel *channel = serv->find_channel(channel_names[i]);
        if (!channel)
        {
            client.reply(ERR_NOSUCHCHANNEL(client.get_nickname(), channel_names[i]));
            return;
        }
        if (!(channel->is_member(&client)))
        {
            client.reply(ERR_NOTONCHANNEL(client.get_nickname(), channel_names[i]));
            return;
        }
        channel->sending(&client, NULL, "PART", NULL, &channel_names[i]);
        channel->remove_member(&client);
        channel->print_members();
        if (channel->members.size() == 0)
           serv->remove_channel(channel_names[i]);
    }
}
