#include "Server.hpp"

int stopSignal = 0;

int	Server::acceptConnection(int i) {
	int new_sd;
	if ((new_sd = accept(_sockVec[i], NULL, NULL)) < 0) {
		ERR_SYST("accept()", "failed");
		_serverState = 1;
		return 1;
	}
	MSG_SYST("New incoming connection \t[" RED << new_sd << GRN "]");

	_pollVec.push_back(_pollVec.back());
	_pollVec.back().fd		= new_sd;
	_pollVec.back().events	= POLLIN;

	if (fcntl(new_sd, F_SETFL, O_NONBLOCK) < 0)
		return 1;
	_clientFolder.push_back(ClientFolder(i));
	_clientFolder.back().header.set_config(&this->_config);
	_clientFolder.back().header.set_server_config(&(this->_config.getServerConfig(_sockToConf[i])));
	_clientFolder.back().header.set_send_fd(new_sd);
	return 0;
}

void	Server::closeClientConnection(size_t i) {
	close(_pollVec[i].fd);
	_pollVec[i].fd = -1;
	_pollVec.		erase(_pollVec.begin() + i);
	_clientFolder.	erase(_clientFolder.begin() + i);
}

void Server::interact() {

	_pollVec.		push_back(pollfd());
	_clientFolder.	push_back(ClientFolder());

	generateServerSocket();
	startListen();
	while (_serverState == 0 && !stopSignal)
	{
		if (initPoll())
			break;
		for (size_t i = 0; i < _pollVec.size(); i++) {
			if(_pollVec[i].revents == 0)
				continue;

			if(_pollVec[i].revents & POLLHUP) {
				INFO_SYST("Client" << i, "Hung up");
				closeClientConnection(i);
				break;
			}
			if (_pollVec[i].revents & POLLERR) {
				ERR_SYST("Client " << i,"Error condition");
				closeClientConnection(i);
				break;
			}
			if (_pollVec[i].revents & POLLNVAL) {
				ERR_SYST("Client " << i,"Invalid polling request");
				closeClientConnection(i);
				break;
			}
			if(_pollVec[i].revents != POLLIN && _pollVec[i].revents != POLLOUT
				&& _pollVec[i].revents != POLLPRI) {
				ERR_SYST("revents", _pollVec[i].revents)
				_serverState = 1;
				break;
			}
			if (i < _sockVec.size() && _pollVec[i].fd == _sockVec[i]) {
				if (acceptConnection(i) != 0)
					break;
			}
			else {
				CLTRESETTIMESTAMP;
				if (CLTSTATE == HEADER && CLTEND == 0 && doRecv(i, 1) != 0) {
					break ;
				}
				if (CLTEND == 1 && CLTSTATE == HEADER) {
					parseHeader(CLTSTR, i);
					CLTHEADER.identify_subconfig();
					CLTEND	= 0;
					if (CLTSTR.size() >= 32000){
						CLTHEADER.generate_error_response(431);
					}
					CLTSTATE	= BODY;
					if (COUNT_KEY("ERROR")) {
						CLTHEADER.generate_error_response(400);
					}
					if (COUNT_KEY(CONTENT_LENGTH) == 1 && COUNT_KEY(TRANSFER_ENCO) == 0) {
						CLTTREC = atoi(GET_PARSE_VALUE(CONTENT_LENGTH).c_str());
						if (CLTTREC == std::string::npos) {
							CLTHEADER.generate_error_response(400);
						}
					}
					else if (COUNT_KEY(TRANSFER_ENCO) == 1 && GET_PARSE_VALUE(TRANSFER_ENCO) == "chunked") {
						CLTCHUNK = 1;
					}
					else {
						CLTTREC = 0;
					}
					if (CLTTREC > ato_octets(CLTHEADER.get_config_val(MAX_BODY_SIZE))) {
						DBG("CLTREC = " << CLTTREC)
						CLTHEADER.generate_error_response(413);
					}
					if (CLTHEADER.get_parsed_header_val("url").size() >= 2048){
						CLTHEADER.generate_error_response(414);
					}
					if (CLTHEADER.err_code != 0) {
						this->_pollVec[i].events = POLLOUT;
						CLTEND = 1;
					}
					CLTSTATE = BODY;
					if ((CLTCHUNK == 0 && CLTTREC == 0) || (CLTCHUNK == 0 && CLTTREC && CLTTREC <= CLTBDSZ))
						CLTEND = 1;

				}
				if (CLTSTATE == BODY && CLTEND == 0 && doRecv(i, 0) != 0) {
					break ;
				}
				if (CLTEND == 1 && CLTSTATE == BODY && CLTHEADER.to_send == 0 && CLTHEADER.is_cgi_on == 0) {
					parsebody(CLTBODY, i);
					if (dispatchMethod(i))
						closeClientConnection(i);
				}
				this->send_request(i);
			}
		}
	}
	if (stopSignal)
		INFO_SYST("Closing server", "[SIGINT]")
	cleaner();
	GOODBYE_MSG
}


int		Server::send_request(int i) {
	if (CLTHEADER.is_cgi_on) {
		CLTHEADER.generate_get_response();
		this->_pollVec[i].events = POLLOUT;
		return (1);
	}
	else if (CLTHEADER.to_send) {
		if (CLTHEADER.write_request()) {
			closeClientConnection(i);
			return (1);
		}
		if (CLTHEADER.is_finished == 1) {
			if (CLTHEADER.err_code == 400 || CLTHEADER.err_code == 408
			|| CLTHEADER.err_code == 431) {
				closeClientConnection(i);
				return (1);
			}

			CLTHEADER.reset_value();
			CLTSTR.	clear();
			CLTBODY.clear();
			CLTEND		= 0;
			CLTCHUNK	= 0;
			CLTTREC		= 0;
			CLTBDSZ		= 0;

			CLTSTATE = HEADER;
			this->_pollVec[i].events = 1;
		}
		else {
			this->_pollVec[i].events = POLLOUT;
		}
		return (1);
	}
	return (0);
}
