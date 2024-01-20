#include "Command.hpp"

Nick::Nick(Server *server) : Command(server) {}

Nick::~Nick() {}

bool Nick::is_valid(std::string nick)
{
    for (size_t i = 0; i < nick.size(); i++)
    {
        char c = nick[i];
        if (c != '[' && c != ']' && c != '\\' && c != '`' && c != '_'
                && c != '^' && c != '{' && c != '}' && c != '|' &&
                !std::isalpha(c))
                return false;
    }
    return true;
}

// void Nick::execute(Client& client, Arguments args)
// {
//     if (!client.has_registered())
//         client.reply(ERR_NOTREGISTERED(client.get_nickname()));
//     else if (args.size() != 1)
//         client.reply(ERR_NEEDMOREPARAMS(client.get_nickname(), "NICK"));
//     else if (serv->is_client(args[0]))
//         client.reply(ERR_NICKNAMEINUSE(client.get_nickname()));
//     else if (!is_valid(args[0]))
//         client.reply("Forbidden characters\r\n");
//     else
//         client.set_nickname(args[0]);
// }

void Nick::execute(Client& client, Arguments args)
{
    if (args.size() != 1)
        client.reply(ERR_NONICKNAMEGIVEN(client.get_nickname()));
    else if (serv->is_client(args[0]))
        client.reply(ERR_NICKNAMEINUSE(client.get_nickname(), args[0]));
    else if (!is_valid(args[0]))
        client.reply(ERR_ERRONEUSNICKNAME(client.get_nickname(), args[0]));
    else if (client.has_registered())
    {
        std::string old_nick = client.get_nickname();
        client.set_nickname(args[0]);
        serv->notify_clients(":" + old_nick + " NICK " + args[0]);
    }
    else
    {
        client.set_nickname(args[0]);
        client.welcome();
    }
}