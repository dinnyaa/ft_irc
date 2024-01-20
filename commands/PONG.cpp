#include "Command.hpp"

Pong::Pong(Server *server) : Command(server) {}

Pong::~Pong() {}

void Pong::execute(Client& client, Arguments args)
{
    if (!client.has_registered())
        client.reply(ERR_NOTREGISTERED(client.get_nickname()));
    else if (args.empty())
        client.reply(ERR_NEEDMOREPARAMS(client.get_nickname(), "PONG"));
    else
        client.sending(RPL_PING(client.get_prefix(), args[0]));
}
