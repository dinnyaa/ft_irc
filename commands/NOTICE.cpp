#include "Command.hpp"

Notice::Notice(Server *server) : Command(server) {}

Notice::~Notice() {}

void Notice::execute(Client& client, Arguments args)
{
    if (!client.has_registered())
    {
        client.reply(ERR_NOTREGISTERED(client.get_nickname()));
        return;
    }
    if (args.size() < 2)
    {
        client.reply(ERR_NEEDMOREPARAMS(client.get_nickname(), "NOTICE"));
        return;
    }
    std::vector<std::string> targets = ft_split(args[0], ",");
    std::string message = args[1].substr(args[1][0] == ':' ? 1 : 0);
    for (size_t i = 2; i < args.size(); ++i)
        message.append(" " + args[i]);
    for(size_t i = 0;i < targets.size(); ++i)
    {
        if (targets[i][0] == '#' || targets[i][0] == '&')
        {
            Channel *channel = serv->find_channel(targets[i]);
            if (!channel)
              //  client.reply(ERR_NOSUCHCHANNEL(client.get_nickname(), targets[i]));
              return ;
            else if (!channel->is_member(&client))
              //  client.reply(ERR_NOTONCHANNEL(client.get_nickname(), targets[i]));
              return ;
            else
                channel->sending(&client, &message, "NOTICE", NULL, &targets[i]);
            return;
        }


        Client* other_client = serv->find_client(targets[i]);
        if (!other_client) 
        {
            //client.reply(ERR_NOSUCHNICK(client.get_nickname(), targets[i]));
            return;
        }
        other_client->sending(RPL_MSG(client.get_prefix(), "NOTICE", targets[i], message));
    }
}