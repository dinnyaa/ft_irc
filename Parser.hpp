#pragma once

#include <vector>
#include <string>
#include <cstring>
#include <iostream>

#include "Channel.hpp"
#include "Replies.hpp"

typedef std::string                   Name;
typedef std::vector<std::string>      Arguments;

class Server;

class Parser
{
public:
    Parser();
    bool is_valid();
    void get_command(Name &name, Arguments &args);
    ~Parser();
    void operator()(std::string str);

private:
    char *strtrim(char *str);

private:
    Name name;
    Arguments args;
};
