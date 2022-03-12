#include "Server.hpp"

int main(int ac, char **av) {
	signal(SIGINT, signalHandler); 
	signal(SIGPIPE, SIG_IGN);

	const char * path = (ac == 2)? av[1]
								 : "config/default.conf";
	Server serv(path);
	serv.interact();

	return 0;
}
