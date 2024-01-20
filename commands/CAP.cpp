#include "Command.hpp"

Cap::Cap(Server *server) : Command(server) {}

Cap::~Cap() {}


void Cap::execute(Client& client, Arguments args)
{
    (void)client;
    (void)args;
}