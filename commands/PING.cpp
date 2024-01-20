#include "Command.hpp"

Ping::Ping(Server *server) : Command(server) {}

Ping::~Ping() {}

void Ping::execute(Client& client, Arguments args)
{   
    if (!client.has_registered())
    	client.reply(ERR_NOTREGISTERED(client.get_nickname()));
   	else if (args.size() == 0)
		client.reply(ERR_NEEDMOREPARAMS(client.get_nickname(), "PING"));
	else
   		client.sending(RPL_PING(client.get_prefix(), args[0]));
}
