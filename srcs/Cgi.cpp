#include "Cgi.hpp"

Cgi::Cgi() {
	_pid = -1;
	_wait_time = 0;
}

Cgi::Cgi(Cgi const & src) {

	this->clear();

	this->_parsed_header = src._parsed_header;
	this->_subconfig_id = src._subconfig_id;
	this->_data = src._data;
	this->_config = src._config;
}

Cgi::Cgi(MAP_STRING header, ServerConfig const * config, int subconfig, url_extract data)
	: _parsed_header(header),
	  _subconfig_id(subconfig),
	  _data(data) {
		  this->_config = config;

}

Cgi::~Cgi() {
	this->clear();
}

Cgi & Cgi::operator=(Cgi const &src) {
	this->_parsed_header	= src._parsed_header;
	this->_subconfig_id		= src._subconfig_id;
	this->_data				= src._data;
	this->_config			= src._config;
	return *this;
}

void			Cgi::set_url(url_extract data) {
	this->_data = data;
}

void			Cgi::set_header(MAP_STRING header) {
	this->_parsed_header = header;
}

void			Cgi::set_servConfig(ServerConfig const * config) {
	this->_config = config;
}

void			Cgi::set_id_sub(int sub) {
	this->_subconfig_id = sub;
}

void			Cgi::sys_call() {
	std::string body = this->get_parsed_header_val("body");
	dup2(this->out_fd[0], 0);
	dup2(this->out_fd[1], 1);
	std::cout << body << std::endl;
	close(this->out_fd[1]);
	dup2(this->_out_fd, 1);
	dup2(this->_out_fd, 2);
	execve(this->_arg_val[0], this->_arg_val, this->_env_val);
	close(this->_out_fd);
	close(this->out_fd[0]);
	close(this->out_fd[1]);
	exit(1);
}

void			Cgi::clear() {
	if (this->_cgi_env.size()) {
		for (size_t i = 0; i < this->_cgi_env.size(); i++) {
			delete[] this->_env_val[i];
		}
		this->_cgi_env.clear();
		delete[] this->_env_val;

		for (size_t i = 0; i < 2; i++) {
			delete[] this->_arg_val[i];
		}
		delete[] this->_arg_val;
	}
}

std::string		Cgi::get_config_val(std::string key) {
	return ((SUB_ID == -1) ? CONFIG(key, std::string()) : SUBCONFIG(key, std::string()));
}

void Cgi::convert_env_cgi(){
	std::string tmp;
	int i, map_size = this->_cgi_env.size();
	this->_env_val	= new char*[map_size + 1];
	MAP_STRING::iterator it = _cgi_env.begin();
	for(i = 0; i < map_size; i++){
		tmp = it->first + "=" +it->second;
		this->_env_val[i] = new char[tmp.size()+ 1];
		strcpy(this->_env_val[i], tmp.c_str());
		it++;
	}
	this->_env_val[i] = NULL;
	std::string path	= this->_config->getServerConfig()->getMatchCgi(this->_data._ext).second;
	std::string val		= this->_data._path.c_str();

	this->_arg_val		= new char*[3];
	this->_arg_val[0]	= new char[path.size()+ 1];
	strcpy(this->_arg_val[0], path.c_str());
	this->_arg_val[1]	= new char[val.size()+ 1];
	strcpy(this->_arg_val[1], val.c_str());
	this->_arg_val[2]	= NULL;
}


void		Cgi::set_cgi_env() {
	this->_cgi_env["AUTH_TYPE"]			= "";
	this->_cgi_env["CONTENT_LENGTH"]	= this->get_parsed_header_val("Content-Length");
	this->_cgi_env["CONTENT_TYPE"]		= this->get_parsed_header_val("Content-Type");
	this->_cgi_env["GATEWAY_INTERFACE"]	= "CGI/1.1";
	this->_cgi_env["PATH_INFO"]			= this->_data._file_name;
	this->_cgi_env["PATH_TRANSLATED"]	= this->_data._path;
	this->_cgi_env["QUERY_STRING"]		= _query_string;
	this->_cgi_env["REQUEST_METHOD"]	= this->get_parsed_header_val("method");
	this->_cgi_env["REQUEST_URI"]		= this->get_parsed_header_val("url");
	this->_cgi_env["SERVER_NAME"]		= CONFIG(SERVER_NAME, std::string());
	this->_cgi_env["SERVER_PORT"]		= CONFIG(LISTEN, std::string());
	this->_cgi_env["SERVER_PROTOCOL"]	= this->get_parsed_header_val("protocol_version");
	this->_cgi_env["REDIRECT_STATUS"]	= "200";
	this->_cgi_env["DOCUMENT_ROOT"]		= get_config_val(ROOT);
	this->_cgi_env["REMOTE_HOST"]		= this->get_parsed_header_val("User-Agent");
	this->_cgi_env["SERVER_SOFTWARE"]	= WEBSERV_VERSION;
}

pid_t				Cgi::process_cgi(std::string data) {
	this->_query_string = data;
	this->set_cgi_env();
	this->convert_env_cgi();
	this->_out_fd = open(TMP_FILE.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (_out_fd == -1)
		return -1;

	this->_wait_time = 0;
	if (pipe(this->out_fd) == -1)
		return (-1);
	if ((this->_pid = fork()) < 0)
		return (-1);
	if (this->_pid == 0){
		this->sys_call();
		exit(0);
	}
	else {
		close(_out_fd);
		close(this->out_fd[0]);
		close(this->out_fd[1]);
		_out_fd = 0;
		out_fd[0] = 0;
		out_fd[1] = 0;
		this->clear();
		return (this->_pid);
	}
	return (0);
}

PAIR_STR_INT	Cgi::get_cgi_response(pid_t pid, struct timeval time) {
	PAIR_STR_INT ret;
	struct timeval te;
	GSETTIME;
	ret.first = "";
	ret.second = 0;
	int status = 0;
	int wpid = waitpid(pid, &status, WNOHANG | WUNTRACED);
	if (wpid == 0 ) {
		if ((long long)CONFIG(CGITEMOUT, size_t()) < GGETTIME - GETCGITIME) {
			ret.second = 2;
			kill(pid, SIGKILL);
			return (ret);
		}
		ret.second = -1;
		return (ret);
	}
	std::ifstream file(TMP_FILE.c_str());
	std::stringstream tmp;
	tmp << file.rdbuf();
	ret.first = tmp.str();
	file.close();
	if (status != 0) {
		if (ret.first.size() < 500) {
			INFO_SYST(YEL<< "STATUS","\t[" << RED << status << YEL << "]")
			INFO_SYST(YEL<< "CGI ERROR","\"" << RED << ret.first << YEL << "\"")
		}
	}
	else
		ret.second = 1;

	return (ret);
}

std::string		Cgi::get_parsed_header_val(std::string key) {
	if (this->_parsed_header.count(key) == 0) {
		return ("");
	}
	else {
		return (this->_parsed_header[key]);
	}
}

PAIR_STR_STR		Cgi::process_body(std::string val) {
	PAIR_STR_STR ret_val;
	std::stringstream ret;

	std::string error;
	std::string end;
	std::string code = "200";

	int x = 0;

	if (val.find("\r\n\r\n") == std::string::npos) {
		// INFO_SYST(YEL<< "CGI ERROR","\"" << RED << ret.first << YEL << "\"")
		ret_val.first = "Status: 200\r\n\r\n";
		ret_val.second = val;
		return (ret_val);
	}
	else {
		ret_val.first = val.substr(0, val.find("\r\n\r\n"));
		ret_val.second = val.substr(val.find("\r\n\r\n") + 4);
	}

	while (ret_val.first.find("PHP Notice") != std::string::npos) {
		if (x == 0) {
			ret << "<div class=\"error\" style=\"text-align: center;border: solid red 2px;background: bisque;color: blueviolet;font-weight: bold;\">" << std::endl;
			ret << "<p style=\"text-decoration: underline;\"> Error : </p>" << std::endl;
		}
		ret << "<p>" << std::endl;
		ret << ret_val.first.substr(0, ret_val.first.find("\n")) << std::endl;
		ret_val.first = ret_val.first.substr(ret_val.first.find("\n") + 1);
		ret << "</p>" << std::endl;
		x++;
	}
	if (x) {
		ret << "</div>" << std::endl;
		ret << "</br>" << std::endl;
	}
	ret_val.second = ret.str() + ret_val.second;
	return (ret_val);
}
