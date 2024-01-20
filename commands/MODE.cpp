#include "Command.hpp"

bool is_number(const std::string& s)
{
    try {
        std::stoi(s);
    } catch (std::exception& e) {
        return false;
    }
    return true;
}

Mode::Mode(Server *server) : Command(server) {}

Mode::~Mode() {}

//+|-]|o|p|s|i|t|n|b|v
void Mode::exec_o(Client& client, Channel *channel, char sign, Arguments args)
{
    if (args.size() != 3)
    {
          client.reply("Bad -o  parameters");
          return;
    }
    Client* other_client = serv->find_client(args[2]);
    if (!other_client) 
    {
        client.reply(ERR_NOSUCHNICK(client.get_nickname(), args[2]));
        return;
    }
    if (!channel->is_member(other_client))
    {
        client.reply(ERR_USERNOTINCHANNEL(other_client->get_nickname(), client.get_nickname(), channel->get_name()));
        return;
    }

    if (sign == '-' && channel->is_operator(other_client))
    {
        client.sending(RPL_MODE(client.get_prefix(), channel->get_name(), "-o"));
        channel->remove_operator(other_client);
    }
    else if (sign == '-')
        client.reply(args[2] + " is not operator.");
    else  if (sign == '+' && channel->is_operator(other_client))
        client.reply(args[2] + " is already operator.");
    else if (sign == '+')
    {
        client.sending(RPL_MODE(client.get_prefix(), channel->get_name(), "+o"));
        channel->add_operator(other_client);
    }
}
void Mode::exec_k(Client& client, Channel *channel, char sign, Arguments args)
{
    if (args.size() != 3)
        client.reply(ERR_NEEDMOREPARAMS(client.get_nickname(), "MODE"));
    if(sign == '+')
    {
        channel->set_password(args[2]);
        client.sending(RPL_MODE(client.get_prefix(), channel->get_name(), "+k"));
    }
    else if (sign == '-')
    {
        if(args[2] == channel->get_password())
        {
            channel->set_password("");
            client.sending(RPL_MODE(client.get_prefix(), channel->get_name(), "-k"));
        }
        else
            client.reply(ERR_PASSWDMISMATCH(args[2]));
    }

}

void Mode::execute(Client& client, Arguments args)
{
    if(!client.has_registered())
    {
        client.reply(ERR_NOTREGISTERED(client.get_nickname()));
        return ;
    }
    else if (args.size() < 2)
    {
        client.reply(ERR_NEEDMOREPARAMS(client.get_nickname(), "MODE"));
        return ;
    }
    else if(!(args[1][0] == '+' || args[1][0] == '-'))
    {
        client.reply("Bad option error");
        return ;
    }
    else if(args[1].size() >  2)
    {
        client.reply("Bad option parameters");
        return ;
    }

    Channel *channel = serv->find_channel(args[0]);
    if (!channel)    
        client.reply(ERR_NOSUCHCHANNEL(client.get_nickname(), args[0]));
    else if (!channel->is_operator(&client))
        client.reply((ERR_CHANOPRIVSNEEDED(client.get_nickname(), args[0])));
    else 
    {
        char sign = args[1][0];
        switch (args[1][1])
        {
            case 'o':
                exec_o(client, channel, sign, args);
                break;
            case 'k':
                exec_k(client, channel, sign, args);
                break;
            default:
                client.reply("Bad option parameters");
        }
    } 
    //Parameters: <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>]
         //      [<ban mask>]
}