#include "Server.hpp"

int Server::generateServerSocket() {
	for (size_t i = 0; i < NB_BLOCK_SERV; i++) {
		memset(&_sockIn, 0, sizeof(_sockIn));
		int on = 1;
		_sockVec.		push_back(socket(AF_INET, SOCK_STREAM, 0));
		_pollVec.		push_back(pollfd());
		_clientFolder.	push_back(ClientFolder());

		if (_sockVec.back() < 0) {
			ERR_SYST("socket()", "failed")
			_serverState = 1;
			return 1;
		}
		fcntl(_sockVec.back(), F_SETFL, O_NONBLOCK); /* Set the socket to non-blocking */

		if (setsockopt(_sockVec.back(), SOL_SOCKET,  SO_REUSEADDR,(char *)&on, sizeof(on)) < 0) /* Allow socket descriptor to be reuseable */ {
			ERR_SYST("setsockopt()", "failed");
			_serverState = 1;
			return 1;
		}

		if (SERVER_IP(i) != "localhost")
			inet_pton(AF_INET, SERVER_IP(i).c_str(), &_sockIn.sin_addr.s_addr); /* Adresse IP du poste e3r2p12 */
		else
			inet_pton(AF_INET, "127.0.0.1", &_sockIn.sin_addr.s_addr);

		_sockIn.sin_family	= AF_INET;              /* Protocole familial (IP) */
		_sockIn.sin_port	= htons(SERVER_PORT(i));   /* Listage du port */

		if (bind(_sockVec.back(), (struct sockaddr *)&_sockIn, sizeof(_sockIn)) < 0)  /* Bind the socket */ {
			ERR_SYST("bind()", "failed")
			_serverState = 1;
		}
		_pollVec[i].fd		= _sockVec.back();
		_pollVec[i].events	= POLLIN; // If you use POLLIN only, poll() will return if there is data or urgent data to read.
									//If you use POLLPRI only, poll() will return only if there is urgent data to read, but ignore normal data.
	}
	return 0;
}

int Server::startListen() {
	if (_serverState != 0)
		return 1;
	int index = 0;

	for (size_t i = 0; i < _sockVec.size(); i++) {
		if (listen(_sockVec[i], BACKLOG(i)) < 0) {
			ERR_SYST("listen()", "failed")
			closeClientConnection(i);
			_sockVec.erase(_sockVec.begin() + i);
			i--;
			index++;
		}
		else
			_sockToConf[i] = i + index;
	}
	return 0;
}


int	Server::initPoll() {
	int rc = poll(_pollVec.data(), _pollVec.size(), TIMEOUT);
	if (rc == 0){
		for (size_t i = NB_BLOCK_SERV + 1; i < _pollVec.size(); i++){
			struct timeval te;
			GSETTIME;
			if ((long long)TIMEOUT < GGETTIME - CLTGETTIMESTAMP){
				CLTHEADER.generate_error_response(408);
				CLTSTATE=BODY;
				CLTEND=1;
				this->_pollVec[i].events = POLLOUT;
			}
		}
	}
	if (rc < 0) {
		if (rc < 0) {
			ERR_SYST("poll()", "failed")
		}
		else if (rc == 0) {
			ERR_SYST("poll()", "timed out. End program.")
		}
	}
	return 0;
}
