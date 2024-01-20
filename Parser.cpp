#include "Parser.hpp"

Parser::Parser()
{
}

void Parser::operator()(std::string str)
{
    str = strtrim(const_cast<char*>(str.c_str()));
    size_t i = str.find(':');
    std::string to_split;
    if (i != std::string::npos && i != 0)
        to_split = str.substr(0, i - 1);
    else
        to_split = str;
    args = ft_split(to_split, " ");
    name = args[0];
    args.erase(args.begin());
    if (i != std::string::npos)
    {
        std::string last_param = str.substr(i);
        args.push_back(last_param);
    }
}

Parser::~Parser()
{
}

void Parser::get_command(Name &name, Arguments &args)
{
    name = this->name;
    args = this->args;
}

bool Parser::is_valid()
{
    if (name == "PRIVMSG" || name == "NOTICE"
        || name == "PASS" || name == "USER"
        || name == "NICK" || name == "JOIN"
        || name == "KICK" || name == "PART"
        || name == "QUIT" || name == "MODE"
        || name == "PING" || name == "PONG"
        || name == "CAP")
        return 1;
    return 0;
}

char *Parser::strtrim(char *str)
{
    for(int i = 0; str[i] != '\0'; i++)
        if (str[i] == '\r' || str[i] == '\n')
            str[i] = '\0';
    return str;
}
