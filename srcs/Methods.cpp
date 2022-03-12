#include "Server.hpp"

static void	exportLog(std::string const & header) {
	std::string log_str = header;
	while (log_str.find("\r\n") != std::string::npos) {
		log_str = log_str.replace(log_str.find("\r\n"), 2, "\t\t");
	}
    std::streambuf *coutbuf = std::cout.rdbuf();
   	std::ofstream outlog(HEADER_LOG_FILE, std::ios_base::app);
    std::cout.rdbuf(outlog.rdbuf());
	std::cout << log_str << std::endl;
    std::cout.rdbuf(coutbuf);
}

int	Server::parseHeaderUtil(std::string & header, size_t const i, std::string const & key, size_t x, size_t step) {
	if (x == NPOS || x > header.size() || !x)
		return 1;
	SET_PARSE_VALUE(key, header.substr(0, x));
	if (header.size() < x + step)
		return 1;
	header = header.substr(x + step);
	if (key != "method" && key != "url" && key != "protocol_version") {
		std::string value = GET_PARSE_VALUE(key);
		truncSpaces(value);
		SET_PARSE_VALUE(key, value);
	}
	return 0;
}

void Server::parseHeader(std::string header, size_t const i) {
	exportLog(header);
	if (header.empty() || header[0] == ' ' || header.find("\r\n") == NPOS) {
		SET_PARSE_VALUE("ERROR", "ERROR");
		return;
	}
	if (header.find_first_of("\t\v") != NPOS
	&& header.find_first_of("\t\v") < header.find("\r\n")) {
		SET_PARSE_VALUE("ERROR", "ERROR");
		return;
	}
	if (parseHeaderUtil(header, i, "method", header.find(' '), 1)) {
		SET_PARSE_VALUE("ERROR", "ERROR");
		return;
	}
	if (parseHeaderUtil(header, i, "url", header.find(' '), 1)) {
		SET_PARSE_VALUE("ERROR", "ERROR");
		return;
	}
	if (parseHeaderUtil(header, i, "protocol_version", header.find("\r\n"), 2)
		|| GET_PARSE_VALUE("protocol_version").find_first_of(" \t\v\f") != NPOS) {
		SET_PARSE_VALUE("ERROR", "ERROR");
		return;
	}
	std::string key, tmp;
	while (header.size() && header.find("\r\n") != NPOS) {
		if (header.find(':') == NPOS) {
			SET_PARSE_VALUE("ERROR", "ERROR");
			return ;
		}
		key = header.substr(0, header.find(':'));
		if (key.find_first_of(" \t\n\r\v\f") != NPOS
				|| key.empty()
				|| GET_PARSE_VALUE(key).size()
				|| header.size() == key.size() + 1) {
			SET_PARSE_VALUE("ERROR", "ERROR");
			return ;
		}
		header = header.substr(header.find(':') + 1);
		if (header.empty()) {
			SET_PARSE_VALUE("ERROR", "ERROR");
			return ;
		}
		parseHeaderUtil(header, i, key, header.find("\r\n"), 2);
	}
}

void Server::parsebody(std::string body, size_t const i){
	SET_PARSE_VALUE("body", body);
}

int	Server::dispatchMethod(size_t const i){
	int ret = 0;

	if (COUNT_KEY("Host") == 0 || GET_PARSE_VALUE("Host").empty()
		|| !isValidHost(GET_PARSE_VALUE("Host"))) {
		ret = CLTHEADER.generate_error_response(400);
	}
	else if (GET_PARSE_VALUE("Content-Length").size()
		&& GET_PARSE_VALUE("Content-Length").find_first_not_of("0123456789") != std::string::npos) {
		ret = CLTHEADER.generate_error_response(400);
	}
	else if (GET_PARSE_VALUE("ERROR") == "ERROR") {
		ret = CLTHEADER.generate_error_response(400);
	}
	else if (!isMethodAllowed(GET_PARSE_VALUE("method"), CLTHEADER.get_config_val("methods"))) {
		ret = CLTHEADER.generate_error_response(405);
	}
	else if (GET_PARSE_VALUE("url").empty()) {
		ret = CLTHEADER.generate_error_response(400);
	}
	else if ((GET_PARSE_VALUE("method") == "GET"
			|| GET_PARSE_VALUE("method") == "POST"
			|| GET_PARSE_VALUE("method") == "DELETE")
			&& !isValidHTTPversion(GET_PARSE_VALUE("protocol_version"))) {
		if (GET_PARSE_VALUE("protocol_version").find("HTTP/") != std::string::npos)
			ret = CLTHEADER.generate_error_response(505);
		else
			ret = CLTHEADER.generate_error_response(400);
	}
	else {
		if (GET_PARSE_VALUE("method") == "GET")
			ret = CLTHEADER.generate_get_response();
		else if (GET_PARSE_VALUE("method") == "POST")
			ret = CLTHEADER.post_request();
		else if (GET_PARSE_VALUE("method") == "DELETE")
			ret = CLTHEADER.delete_request();
		else {
			ret = CLTHEADER.generate_error_response(405);
		}
	}
	this->_pollVec[i].events = POLLOUT;

	return ret;
}

void Server::requestGet(int i){
	CLTHEADER.generate_get_response();
}

void Server::requestPost(int i){
	CLTHEADER.post_request();
}

void Server::requestDelete(int i){
	CLTHEADER.delete_request();
}
