#include "Server.hpp"

void signalHandler(int signum) {
	std::cout << "\r";
	INFO_SYST("Interupt Signal", signum << " received")
	stopSignal = 1;
}
