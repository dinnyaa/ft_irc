#include "Command.hpp"

Pass::Pass(Server *server) : Command(server) {}

Pass::~Pass() {}

void Pass::execute(Client& client, Arguments args)
{
    if (client.has_passed())
    {
        client.reply(ERR_ALREADYREGISTERED(client.get_nickname()));
        return ;
    }
    if (args.size() != 1)
    {
        client.reply(ERR_NEEDMOREPARAMS(client.get_nickname(), "PASS"));
        return ;
    }
    if (args[0][0] == ':')
        args[0].erase(0, 1);
    if (args[0] != serv->get_password())
        client.reply(ERR_PASSWDMISMATCH(client.get_nickname()));
    else
    {
        client.set_passed();
        client.welcome();
    }
}

// void Pass::execute(Client& client, Arguments args)
// {
//     if (args.size() != 1)
//     {
//         client.reply(ERR_NEEDMOREPARAMS(client.get_nickname(), "PASS"));
//         return ;
//     }

//     if (args[0][0] == ':')
//         args[0].erase(0, 1);  
//     if (args[0] != serv->get_password())
//         client.reply(ERR_PASSWDMISMATCH(client.get_nickname()));
//     else if (client.has_passed())
//         client.reply("You have already registered\r\n");
//     else
//     // {
//         client.set_passed();
//         // if (client.has_registered())
//             // client.reply(RPL_WELCOME(client.get_nickname()));
//     // }
// }