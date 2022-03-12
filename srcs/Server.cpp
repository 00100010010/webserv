#include "Server.hpp"

Server::Server(): _serverState(0), _config() {
	_config.displayFile();
	if (OUTPUT_FLAG) {
		_config.display_log();
		MSG_SYST("[ " << YEL << "default.conf" <<  GRN << " ] <<\n")
		_config.displayPorts(_config);
	}
}


Server::Server(const char * config_file): _serverState(0), _config(config_file) {
	_config.displayFile();
	if (OUTPUT_FLAG) {
		_config.display_log();
		MSG_SYST("[ " << YEL << config_file <<  GRN << " ] <<\n")
		_config.displayPorts(_config);
	}
}

Server::~Server() {}

int Server::doRecv(size_t & i, bool header_mode) {
	int					rc;
	std::string			str;
	std::stringstream	hex_num;

	rc = recv(_pollVec[i].fd, _buffer, sizeof(_buffer), 0);
	if (rc <= 0) {
		if (rc < 0) {
			INFO_SYST("Error recv Closing connection", "[" RED << i << YEL << "] : closed");
			closeClientConnection(i);
			return 1;
		}
		if (rc == 0) {
			INFO_SYST("Closing connection", "[" RED << i << YEL << "] : closed");
			closeClientConnection(i);
			return 1;
		}
	}
	if (rc > 0) {
		_buffer[rc] = '\0';
		if (header_mode) {
			CLTSTR.append(_buffer, rc);
		}
		else {

			if (CLTCHUNK) {
				str.append(_buffer, rc);
				if (CLTSIZECHU == 0) {
					std::string val = str.substr(0, str.find("\r\n"));
					toLower(val);
 					hex_num << std::hex << val;
					hex_num >> CLTSIZECHU;
					if (CLTSIZECHU == 0 && val == "0") {
						CLTSIZECHU = std::string::npos;
					}
					CLTTMPSCHU = 0;
					CLTBUFFCHU = "";
				}
				else {
					CLTBUFFCHU.append(str);
					CLTTMPSCHU += str.size();
					if (CLTTMPSCHU >= CLTSIZECHU) {
						CLTBUFFCHU = CLTBUFFCHU.substr(0, CLTSIZECHU);
						CLTBODY.append(CLTBUFFCHU);
						CLTSIZECHU = 0;
					}
				}
			}
			else {
				CLTBODY.append(_buffer, rc);
				CLTBDSZ += rc;
			}
		}
	}

	memset(_buffer, 0, sizeof(_buffer));
	size_t x;
	if (header_mode) {
		CLTBDSZ = 0;
		if ((x = CLTSTR.find("\r\n\r\n")) != std::string::npos) {

			CLTEND	= 1;
			CLTBODY = CLTSTR.substr(x + 4);
			CLTSTR	= CLTSTR.substr(0, x + 2);
			CLTBDSZ = CLTBODY.size();
		}
	}
	else {
		if (CLTCHUNK) {
			if (CLTSIZECHU == std::string::npos) {
				CLTEND = 1;
				CLTSIZECHU = 0;
				CLTTMPSCHU = 0;
			}
		}
		else if (CLTBDSZ >= CLTTREC) {
			CLTEND = 1;
		}
	}
	return 0;
}
