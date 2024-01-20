#include "Command.hpp"

Kick::Kick(Server *server) : Command(server) {}

Kick::~Kick() {}

void Kick::execute(Client& client, Arguments args)
{
    if(!client.has_registered())
    {
        client.reply(ERR_NOTREGISTERED(client.get_nickname()));
        return ;
    }
    if(args.size() < 1)
    {
        client.reply(ERR_NEEDMOREPARAMS(client.get_nickname(), "KICK"));
        return;
    }
	Channel *channel = serv->find_channel(args[0]);
    //no such channel
    if(!channel)    
    {
        client.reply(ERR_NOSUCHCHANNEL(client.get_nickname(), args[0]));
        return;
    }

    if(!channel->is_member(&client))
    {
        client.reply(ERR_NOTONCHANNEL(client.get_nickname(), args[0]));
    	return;
    }

    if(!channel->is_operator(&client))
    {
        client.reply(ERR_CHANOPRIVSNEEDED(client.get_nickname(), args[0]));
    	return;
    }

	if (client.get_nickname() == args[1])
	{
		client.reply("You can't kick yourself from channel");
		return ;
	}

    Client *other_client = serv->find_client(args[1]);
    if (!other_client) 
    {
        client.reply(ERR_NOSUCHNICK(client.get_nickname(), args[0]));
        return;
    }

    if(!channel->is_member(other_client))
    {
        client.reply(ERR_USERNOTINCHANNEL(client.get_nickname(), args[1], args[0]));
      return;
    }

    std::string reason = "No reason specified.";
    if (args.size() > 2)
    {
        reason = args[2].substr(args[2][0] == ':' ? 1 : 0);
        for (size_t i = 3; i < args.size(); ++i)
            reason.append(" " + args[i]);
    }
    channel->sending(other_client, &reason, "KICK", &client, &args[0]);   
    channel->remove_member(other_client);
}