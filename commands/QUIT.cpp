#include "Command.hpp"
#include <string>

Quit::Quit(Server *server) : Command(server) {}

Quit::~Quit() {}

void Quit::execute(Client& client, Arguments args)
{
    std::string reason;
    if (args.size())
    {
        reason = args[0].substr(args[0][0] == ':' ? 1 : 0);
        for (size_t i = 1; i < args.size(); ++i)
            reason.append(" " + args[i]);
    }
    client.sending(RPL_QUIT(client.get_prefix(), reason));
    //serv->remove_from_channels(&client);
    client.quit = 1;
}
