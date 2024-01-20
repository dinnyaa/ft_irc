#include "Server.hpp"

int main(int argc , char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Arguments needed! Try\n";
        std::cout << "./ircserv <port> <password>\n";
        return (1);
    }
    Server server(argv[1], argv[2]);
    while(true)
    {
        server.connect_clients();
        server.handle_IO();
    }
	return 0;
}
