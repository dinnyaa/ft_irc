#include "Command.hpp"

User::User(Server *server) : Command(server) {}

User::~User() {}

// void User::execute(Client& client, Arguments args)
// {
//     if (!client.has_passed())
//         client.reply(ERR_NOTREGISTERED(client.get_nickname()));
//     else if (args.size() < 4)
//         client.reply(ERR_NEEDMOREPARAMS(client.get_nickname(), "USER"));
//     else if (client.has_registered())
//         client.reply(ERR_ALREADYREGISTERED(client.get_nickname()));
//     else
//     {
//         client.set_nickname(args[0]);
//         client.set_hostname(args[1]);
//         client.set_username(args[3]);
//        // if (client.has_registered())
//        // {
//             client.reply(RPL_WELCOME(client.get_nickname()));
//             std::cout << client.get_nickname() << " connected to server\n";
//        // }
//     }
// }

void User::execute(Client& client, Arguments args)
{
    if (args.size() < 4)
        client.reply(ERR_NEEDMOREPARAMS(client.get_nickname(), "USER"));
    else if (client.has_registered())
        client.reply(ERR_ALREADYREGISTERED(client.get_nickname()));
    else
    {
        client.set_username(args[0]);
        client.set_hostname(args[2]);
        client.welcome();
    }
}