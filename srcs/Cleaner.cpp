#include "Server.hpp"

void Server::cleaner() {
	for (size_t i = 0; i < _pollVec.size(); i++) {
		if(_pollVec[i].fd >= 0)
			close(_pollVec[i].fd);
	}
	_pollVec.		clear();
	_clientFolder.	clear();
    _sockVec.		clear();
}
