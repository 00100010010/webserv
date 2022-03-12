/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ifaveur <ifaveur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 11:36:10 by ifaveur           #+#    #+#             */
/*   Updated: 2021/11/30 17:08:05 by ifaveur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Header.hpp"

Header::Header(): _cgi() {
	this->reset_value();
	this->_config		= NULL;
	// this->_cgi			= NULL;
	this->_send			= 0;
	this->to_send		= 0;
	this->is_finished	= 0;
	this->is_cgi_on		= 0;
	this->err_code		= 0;
	_pid_cgi			= -1;
	this->_time			= 0;
}


Header::~Header() {
	remove(TMP_FILE.c_str());
}

int Header::init() {
	if (this->check_file_name())
		return 1;
	return (0);
}

void			Header::reset_value() {
	remove(TMP_FILE.c_str());
	FILE_NAME			= "";
	FILE_PATH			= "";
	FILE_EXT			= "";
	FILE_FOLDER			= "";
	TMP_FILE			= "";
	AUTO_INDEX			= 0;
	this->_mime			= "";
	this->_status		= "";
	this->_code			= 0;
	this->_read			= 0;
	this->_file_buffer	= "";
	this->_subconfig_id	= -1;
	this->_query_string	= "";
	this->_send			= 0;
	this->_parsed_header.clear();
	this->_config		= this->_tmp_config;
	this->to_send		= 0;
	this->is_cgi_on		= 0;
	this->is_finished	= 0;
	this->_time			= 0;
	_pid_cgi			= -1;
}

std::string		Header::generate_tmp_file() {
	DIR *dir;
	struct dirent *ent;
	int last	= 0;
	int tmp		= 0;
	if ((dir = opendir ("tmp")) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			tmp = atoi(ent->d_name);
			if (tmp > last)
				last = tmp;
		}
		closedir (dir);
	}
	else {
		mkdir("tmp", 0777);
	}
	std::stringstream ss;
	last++;
	ss << last;
	std::string name = "tmp/" + ss.str();
	std::ofstream file(name.c_str());
	file.close();
	return (name);
}

void 			Header::set_config(Config const *conf){
	this->_conf = conf;
}


void			Header::set_parsed_header_val(std::string key, std::string val) {
	this->_parsed_header[key] = val;
}

std::string		Header::get_parsed_header_val(std::string key) {
	if (this->_parsed_header.count(key) == 0) {
		return ("");
	}
	else {
		return (this->_parsed_header[key]);
	}
}

void			Header::identify_subconfig() {
	this->_tmp_config = _config;
	std::string tmp = this->_parsed_header["Host"];
	tmp = tmp.find(":") != std::string::npos ? tmp.substr(0, tmp.find(":")) : tmp;
	for (size_t i = 0; i < _conf->size(); i++){
		if (this->_conf->getServerValue(i, SERVER_NAME, std::string()) == tmp) {
			set_server_config(&_conf->getServerConfig(i));
			break;
		}
	}
	this->set_file_name();
	this->set_extension();
	this->set_mime();
	this->set_path();

	this->_subconfig_id = this->_config->getmatchLocation(this->_data);
}

void			Header::set_server_config(ServerConfig const * config) {
	this->_config = config;
}

size_t			Header::count_key(std::string key) {
	return (this->_parsed_header.count(key));
}

void			Header::set_send_fd(int x) {
	this->fd = x;
}

size_t			Header::get_int_config_value(std::string key) {
	return ((SUB_ID == -1) ? CONFIG(key, size_t()) : SUBCONFIG(key, size_t()));
}

std::string		Header::get_config_val(std::string key) {
	return ((SUB_ID == -1) ? CONFIG(key, std::string()) : SUBCONFIG(key, std::string()));
}

bool	Header::is_on(std::string key) {
	std::string tmp = get_config_val(key);
	if (tmp == "on") {
		return true;
	}
	else {
		return (false);
	}
}

int							Header::write_request() {
	int n = 0;
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(fd, &rfds);
	std::string str;

	if (this->step == 0) {
		str = this->_header;
		if (FD_ISSET(fd, &rfds)) {
			size_t x = (str.size() - this->size_send < 5000) ? str.size() - this->size_send : 5000;
			std::string tmp(str.c_str() + this->size_send, x);
			n = send(fd, tmp.c_str() , tmp.size(), MSG_DONTWAIT);
			if (n > 0) {
				this->size_send += x;
			}
			else
				return (1);
			if (x == str.size()) {
				this->step = 1;
				this->size_send = 0;
			}
		}
	}
	else if (this->_body.size() > this->size_send) {
		str = this->_body;
		if (str.size()) {
			if (FD_ISSET(fd, &rfds)) {
				size_t x = (str.size() - this->size_send < 5000) ? str.size() - this->size_send : 5000;
				std::stringstream hex_size;
				hex_size << std::hex << x;
				std::string tmp(str.c_str() + this->size_send, x);
				std::string val = hex_size.str() + "\r\n" + tmp + "\r\n";
				n = send(fd, val.c_str() , val.size(), MSG_DONTWAIT);
				if (n > 0) {
					this->size_send += x;
				}
				else
					return (1);
				this->step++;
			}
		}
	}
	else {
		if (FD_ISSET(fd, &rfds)) {
			std::string val = "0\r\n\r\n\r\n";
			n = send(fd, val.c_str() , val.size(), MSG_DONTWAIT);
			this->is_finished = 1;
			if (n <= 0) {
				return (1);
			}
		}
		else
			return (1);
	}
	return (0);
}

int			 Header::generate_get_response() {
	std::stringstream	ret;
	std::string			tmp_head;
	time_t				now = time(0);



	if (this->is_cgi_on == 0 && this->init())
		return 1;
	if (CONFIG(ALL_DOWNLOAD, std::string()) != "default" && CONFIG(ALL_DOWNLOAD, std::string()) == FILE_FOLDER && FILE_NAME.size())
		this->_mime = "application/octet-stream";
	std::pair<bool, std::string> cgi_pair = this->_config->getServerConfig()->getMatchCgi(FILE_EXT);
	if (this->_send == 0) {
		if (!this->is_cgi_on) {
			if (cgi_pair.first && this->_mime == "not_found") {
				TMP_FILE = this->generate_tmp_file();
				this->_cgi.set_url(this->_data);
				this->_cgi.set_header(this->_parsed_header);
				this->_cgi.set_servConfig(this->_config);
				this->_cgi.set_id_sub(this->_subconfig_id);

				_pid_cgi = this->_cgi.process_cgi(this->_query_string);
				if (_pid_cgi == -1) {
					return this->generate_error_response(500);
				}
				this->is_cgi_on = 1;
				this->_time = 0;
				SETCGITIME;
				return 0;
			} else if (!cgi_pair.first && this->_mime == "not_found") {
				struct stat test;
				if (!(stat(FILE_PATH.c_str(), &test) == -1)) {
					if (this->is_on(DOWNLOAD)) {
						return this->generate_error_response(501);
					}
					else
						this->_mime = "application/octet-stream";
				}
			}
		}
		else {
			PAIR_STR_STR	cgi_ret;
			PAIR_STR_INT	check = this->_cgi.get_cgi_response(_pid_cgi, this->_timestamp);
			if (check.second == -1)
				return (0);
			if (check.second == 1) {
				cgi_ret = this->_cgi.process_body(check.first);
				std::string code;
				if (cgi_ret.first.find("Status:") != std::string::npos) {
					code		= cgi_ret.first.substr(cgi_ret.first.find("Status:") + 8, 3);
					this->_code = atoi(code.c_str());
				}
				else {
					this->_code = 200;
				}
				tmp_head			= cgi_ret.first;
				this->_file_buffer	= cgi_ret.second;
				this->_mime			= "text/html";
				this->_read			= 1;
				this->is_cgi_on		= 0;
			}
			else {
				this->is_cgi_on = 0;
			}
			if (check.second == 0) {
				return this->generate_error_response(500);
			}
			else if (check.second == 2) {
				return this->generate_error_response(504);
			}
			if (this->_code == 500) {
				return this->generate_error_response(500);
			}
		}

		this->set_file_content();
		if (this->_send == 1)
			return 0;

		ret << VERSION_HTTP " "	<< this->_code << " " << GET_ERROR(this->_code) << std::endl;
		ret << "Content-Type: "		<< this->_mime << std::endl;
		ret << "Date: "				<< asctime(gmtime(&now));
		ret << "Transfer-Encoding: chunked" << std::endl;
		ret << "Server: "			<< WEBSERV_VERSION << std::endl;
		if (tmp_head.size()) {
			ret << tmp_head.c_str() << std::endl;
		}
		ret << std::endl;

		this->_header = ret.str();
		this->_body = this->_file_buffer;
		this->step = 0;
		this->size_send = 0;
		this->to_send = 1;
	}
	return 0;
}


std::vector<std::string> Header::split(std::string val) {
	std::vector<std::string> ret;
	std::string tmp;
	if (val.find("\r\n\r\n") != std::string::npos) {
		tmp				= val.substr(0, val.find("\r\n\r\n"));
		val				= val.substr(val.find("\r\n\r\n") + 4);
		val				= val.substr(0, val.size() - 2);
	}
	else {
		tmp = val;
	}
	ret.push_back(tmp);
	ret.push_back(val);
	return (ret);
}

void	Header::upload(std::string name, std::string content) {
	std::string upload_folder = MY_CONFIG(ROOT) + FILE_FOLDER;
	mkdir(upload_folder.c_str(), 0777);
	std::string upload_name = upload_folder + name;

	upload_name = this->clear_path(upload_name);
	std::ofstream out(upload_name.c_str());
	if (!out) {
		out.close();
		return ;
	}
	out << content;
	out.close();
}

int		Header::parse_body_request() {
	std::string form_type = this->get_parsed_header_val("Content-Type");
	if (form_type.size() > 0) {
		if (form_type == "application/x-www-form-urlencoded" || form_type == "text/plain") {
			return (-1);
		}
		else if (form_type.find("multipart/form-data") != std::string::npos) {
			std::string boundary;
			if (form_type.find("boundary=") != std::string::npos)
				boundary = "--" + form_type.substr(form_type.find("boundary=") + 9);
			else
				boundary = "error";
			std::string body = this->get_parsed_header_val("body");
			while (body.find(boundary) != std::string::npos) {
				MAP_STRING tmp;
				std::string tmp_str;
				tmp_str = body.substr(body.find(boundary) + boundary.size() + 1);
				if (tmp_str.find(boundary) == std::string::npos)
					break;
				body	= tmp_str.substr(tmp_str.find(boundary));
				tmp_str = tmp_str.substr(0, tmp_str.find(boundary));

				std::vector<std::string> test = this->split(tmp_str);
				if (test[0].find("filename=\"") != std::string::npos) {
					std::string filename = test[0].substr(test[0].find("filename=\"") + 10);
					filename = filename.substr(0, filename.find('"'));
					upload(filename, test[1]);
				}
			}
			return (-1);
		}
		else {
			return (this->post_extend());
		}
	}
	else {
		return (this->generate_error_response(204));
	}
	return (0);
}


int		Header::post_extend() {
	struct stat buf;
	this->_send = 1;

	if (this->init())
		return 1;

	if (stat(FILE_PATH.c_str(), &buf) != -1) {
		std::ofstream out(FILE_PATH.c_str(), out.app);
		if (!out) {
			return (this->generate_error_response(500));
		}
		out << this->get_parsed_header_val("body");
		out.close();
		return (this->generate_error_response(200));
	}
	else {
		std::ofstream out(FILE_PATH.c_str());
		if (!out) {
			return (this->generate_error_response(500));
		}
		out << this->get_parsed_header_val("body");
		out.close();
		return (this->generate_error_response(201));
	}
}


int 	Header::delete_request() {

	if (this->init())
		return 1;

	if (isTryingtoEscape(FILE_PATH.c_str()))
		return this->generate_error_response(403);
	else if( remove( FILE_PATH.c_str() ) != 0 )
    	return this->generate_error_response(404);
  	else
    	return this->generate_error_response(204);

	return 0;
}

int		Header::generate_error_response(int code) {
	if (to_send == 1)
		return 0;
	INFO_SYST(YEL<< "CODE","\t[" << RED << code << YEL << "]")
	this->err_code = code;
	std::stringstream ret;
	time_t now = time(0);

	this->_code = code;
	this->_status = GET_ERROR(code);
	this->_mime = "text/html";
	this->set_error(this->_code);
	ret << VERSION_HTTP " "			<< this->_code << " " << this->_status << std::endl;
	ret << "Content-Type: "			<< this->_mime << std::endl;
	ret << "Date: "					<< asctime(gmtime(&now));
	ret << "Transfer-Encoding: chunked" << std::endl;
	ret << "Server: "				<< WEBSERV_VERSION << std::endl;
	ret << "Connection: close" << std::endl;
	ret << std::endl;

	this->_header = ret.str();
	this->_body = this->_file_buffer;
	this->step = 0;
	this->size_send = 0;
	this->to_send = 1;

	return 0;
}

int		Header::redirection(std::pair<std::string, size_t> location) {
	std::stringstream ret;

	this->_code		= location.second;
	this->_status	= GET_ERROR(location.second);
	ret << VERSION_HTTP " "			<< this->_code << " " << this->_status << std::endl;
	ret << "Location: " << location.first << std::endl;
	ret << "Status: " << this->_code << " " << this->_status << std::endl;
	ret << "\r\n\r\n";

	this->_header = ret.str();
	this->_body = "";
	this->step = 0;
	this->size_send = 0;
	this->to_send = 1;
	this->_send = 1;
	return 0;
}

int 	Header::post_request() {
	int ret;
	ret = this->parse_body_request();
	if (ret >= 0) {
		return (ret);
	}
	return this->generate_get_response();
}

void Header::set_extension() {
	if (FILE_NAME.size() > 0) {
		if (FILE_NAME.find('.') != std::string::npos)
			FILE_EXT = FILE_NAME.substr(FILE_NAME.find_last_of('.') + 1);
		else
			FILE_EXT = "";
	}
	else
		FILE_EXT = "";
}

void Header::set_mime() {
	if (FILE_EXT.size() == 0)
		this->_mime = "";
	else
		this->_mime = GET_MIME(FILE_EXT);
}

std::string		Header::clear_path(std::string path) {
	if (path.size() > 0) {
		while (path.find("//") != std::string::npos) {
			path.replace(path.find("//"), 2, "/");
		}
	}
	return (path);
}

void Header::set_file_name() {
	std::string url = this->_parsed_header["url"];
	std::string name = "";
	if (url.find('/') != std::string::npos)
		name = url.substr(url.find_last_of('/') + 1);
	if (!(name.find('?') == std::string::npos)) {
		_query_string = name.substr(name.find('?') + 1);
		name = name.substr(0, name.find('?'));
	}
	if (name.size() == 0)
		FILE_NAME = "";
	else
		FILE_NAME = name;
}

int			Header::check_file_name() {
	std::string url = this->_parsed_header["url"];
	std::string rewrite_rule = MY_CONFIG(REWRITE);
	if (rewrite_rule != DEFAULT_VAL) {
		if (url.find("?") != std::string::npos){
			url = url.substr(0, url.find("?"));
		}
		std::pair<std::string, size_t> check = check_redirection(url, rewrite_rule);
		if (check.second != 0) {
			return this->redirection(check);
		}
	}
	if (FILE_NAME.size() == 0 && this->is_on(AUTOINDEX)) {
		this->auto_index();
	}
	else if (FILE_NAME.size() == 0) {
		FILE_NAME = MY_CONFIG(DEFAULT_PAGE);
		this->set_extension();
		this->set_mime();
		this->set_path();
		AUTO_INDEX = 1;
	}

	if (this->_subconfig_id != -1 && this->_config->getLocation(this->_subconfig_id)->whatLocationType() == Folder) {
		std::string tmp = this->_config->getLocation(this->_subconfig_id)->getName();
		if (MY_CONFIG(ROOT) != CONFIG(ROOT, std::string()))
			if (FILE_FOLDER.find(tmp) != std::string::npos)
				FILE_FOLDER = FILE_FOLDER.substr(FILE_FOLDER.find(tmp) + tmp.size());
		FILE_PATH = MY_CONFIG(ROOT) + FILE_FOLDER + FILE_NAME;
	}
	else {
		if (FILE_FOLDER != "/")
			FILE_PATH = MY_CONFIG(ROOT) + FILE_FOLDER + FILE_NAME;
		else
			FILE_PATH = MY_CONFIG(ROOT) + FILE_NAME;

	}

	FILE_PATH = this->clear_path(FILE_PATH);
	return 0;
}

void			Header::set_path() {
	std::string url		= this->_parsed_header["url"];
	std::string path	= url;
	if (path.find('/') != std::string::npos)
		path = path.substr(0, path.find_last_of('/') + 1);

	if (!(path.find('?') == std::string::npos))
		path = path.substr(0, path.find('?'));
	FILE_FOLDER = path;

}

void		Header::display_parsed() const {
	std::cout << std::endl;
	std::map<std::string, std::string>::const_iterator it  = _parsed_header.begin();
	std::map<std::string, std::string>::const_iterator ite = _parsed_header.end();
	while (it != ite) {
		if ((it->first == "body" && it->second.size() < 5000) || it->first != "body") {
			std::cout << "|";
			std::cout << std::setfill(' ') << std::setw(30);
			std::cout << it->first << " : " << it->second << std::endl;
		}
		it++;
	}
	std::cout << std::endl;
}

void		Header::display_header() const {
	std::cout << std::endl << "header info : " << std::endl;
	std::cout << std::setfill(' ') << std::setw(20) << "FILE_NAME";
	std::cout << ": " << FILE_NAME << std::endl;
	std::cout << std::setfill(' ') << std::setw(20) << "FILE_PATH";
	std::cout << ": " << FILE_PATH << std::endl;
	std::cout << std::setfill(' ') << std::setw(20) << "FILE_FOLDER";
	std::cout << ": " << FILE_FOLDER << std::endl;
	std::cout << std::setfill(' ') << std::setw(20) << "FILE_EXT";
	std::cout << ": " << FILE_EXT << std::endl;
	std::cout << std::setfill(' ') << std::setw(20) << "subconfig id";
	std::cout << ": " << this->_subconfig_id << std::endl;
	std::cout << std::setfill(' ') << std::setw(20) << "mine";
	std::cout << ": " << this->_mime << std::endl;
	std::cout << std::setfill(' ') << std::setw(20) << "code";
	std::cout << ": " << this->_code << std::endl;
	std::cout << std::setfill(' ') << std::setw(20) << "status";
	std::cout << ": " << this->_status << std::endl;
}

int			Header::set_error(int code, int x) {
	this->_mime = "text/html";
	this->_code = code;
	this->_status = GET_ERROR(code);
	std::stringstream tmp_code;
	tmp_code << code;
	std::string val = CONFIG(tmp_code.str(), std::string());
	if (val == DEFAULT_VAL || x) {
		this->_read = 1;
		this->_file_buffer = this->generete_error_page(this->_code, this->_status);
	}
	else {
		FILE_PATH = CONFIG(ROOT, std::string()) + val;
		if (this->set_file_content(1) == READ_ERROR) {
			this->set_error(code, 1);
		}
	}
	return (READ_OK);
}

int	Header::set_file_content(int x) {
	std::ifstream		file_buffer;
	std::stringstream	tmp;
	struct stat			buf;

	if (this->_read)
		return (READ_OK);

	std::string test = clear_path(FILE_FOLDER + FILE_NAME);

	if (isTryingtoEscape(test.c_str())) {
		return (x ? READ_ERROR : this->set_error(403));
	}

	if (FILE_PATH.size() > 0) {
		while (FILE_PATH.find("%20") != std::string::npos) {
			FILE_PATH.replace(FILE_PATH.find("%20"), 3, " ");
		}
	}
	// CHECK OPEN
	file_buffer.open(FILE_PATH.c_str());
	int i = stat(FILE_PATH.c_str(), &buf);
	if (i != -1 && S_ISDIR(buf.st_mode)) {
		return (this->redirection(std::make_pair<std::string, size_t>(test + "/", 301)));
	}

	if (i == -1) {
		if (AUTO_INDEX) {
			std::string test2 = clear_path(MY_CONFIG(ROOT) + FILE_FOLDER);
			i = stat(test2.c_str(), &buf);
			if (i == -1)
				return (x ? READ_ERROR : this->set_error(404));
			else
				return (x ? READ_ERROR : this->set_error(403));
		}
		else
			return (x ? READ_ERROR : this->set_error(404));
	}
	else if(!file_buffer.is_open()) {
		return (x ? READ_ERROR : this->set_error(403));
	}
	else if (this->_code == 0) {
		this->_code		= 200;
		this->_status	= GET_ERROR(200);
	}
	tmp << file_buffer.rdbuf() << std::endl;
	this->_file_buffer	= tmp.str();
	this->_read			= 1;
	file_buffer.close();
	return (READ_OK);
}

std::string	Header::generete_error_page(int code, std::string message) {
	std::stringstream page;
	page << "<html>" << std::endl;
	page << "<head><title>" << code << " " << message << "</title></head>" << std::endl;
	page << "<body bgcolor=\"white\">" << std::endl;
	page << "<center><h1>" << code << " " << message << "</h1></center>" << std::endl;
	page << "<hr><center>" << WEBSERV_VERSION << "</center>" << std::endl;
	page << "</body>" << std::endl;
	page << "</html>" << std::endl;
	return (page.str());
}

void	Header::auto_index() {
	DIR					*dir;
	struct dirent		*ent;
	std::stringstream 	page;
	std::string			upload_folder = MY_CONFIG(ROOT) + FILE_FOLDER;

	this->_code = 200;
	this->_status = GET_ERROR(200);
	this->_mime = "text/html";

	page << "<html>" << std::endl;
	page << "<head>" << std::endl;
	page << "<title> Auto Index </title>" << std::endl;
	page << "<style>" << std::endl;
	page << "td { padding-left: 50px; padding-right: 50px; }" << std::endl;
	page << "</style>" << std::endl;
	page << "</head>" << std::endl;
	page << "<body bgcolor=\"white\">" << std::endl;
	page << "<center><h1>  Auto index </h1></center>" << std::endl;
	page << "<div class=\"link\">" << std::endl;
	page << "<h1> Index of " << FILE_FOLDER << "</h1><hr>" << std::endl;
	page << "<table>" << std::endl;
	if ((dir = opendir (upload_folder.c_str())) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			folder_content(&page, upload_folder + "/" + ent->d_name, 0, ent->d_name);
		}
		closedir (dir);
	}
	else {
		this->_file_buffer = generete_error_page(404, GET_ERROR(404));
		return ;
	}
	page << "</table>" << std::endl;
	page << "</div><hr><center>" << WEBSERV_VERSION << "</center>" << std::endl;
	page << "</body>" << std::endl;
	page << "</html>" << std::endl;

	this->_file_buffer	= page.str();
	this->_read			= 1;
}

void	Header::folder_content(std::stringstream *page, std::string path, int indent, std::string name) {

	DIR				*dir;
	struct dirent	*ent;
	struct stat		buf;
	std::string		to_clear = MY_CONFIG(ROOT) + FILE_FOLDER;
	path = this->clear_path(path);
	to_clear = this->clear_path(to_clear);

	if ((dir = opendir (path.c_str())) != NULL) {
		if (name != ".") {
			if (name == "..") {
				if (path != MY_CONFIG(ROOT) + "..") {

					*page << "<tr>" << std::endl;
					*page << "<td><a style=\"margin-left: " << indent << "px;\" href=\"" << name + "/" << "\">" << name + "/" << "</a></td>"<< std::endl;
					*page << "<td>" << "</td>" << std::endl;
					*page << "<td>" << "</td>" << std::endl;
					*page << "</tr>" << std::endl;
				}
			}
			else {
				std::string test = "";
				if (path.find(to_clear) != std::string::npos)
					test = path.substr(path.find(to_clear) + to_clear.size());
				lstat(path.c_str(), &buf);
				*page << "<tr>" << std::endl;
				if (this->is_on(AUTOINDEXEXTEND) && indent)
					*page << "<td><a style=\"margin-left: " << indent << "px;\" href=\"" << test + "/" << "\">" << name + "/" << "</a></td>"<< std::endl;
				else
					*page << "<td><a style=\"margin-left: " << indent << "px;\" href=\"" << name + "/" << "\">" << name + "/" << "</a></td>"<< std::endl;
				*page << "<td>" << ctime(&buf.st_mtime) << "</td>" << std::endl;
				// buf.st_mtim = test;
				*page << "<td>" << "..."  << "</td>" << std::endl;
				*page << "</tr>" << std::endl;
			}
		}
		if (name != "." && name != "..") {
			while ((ent = readdir (dir)) != NULL) {
				if (this->is_on(AUTOINDEXEXTEND)) {
					if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
						folder_content(page, path + "/" + ent->d_name, indent + 20, ent->d_name);
				}
			}
		}
		closedir (dir);
	}
	else {
		lstat(path.c_str(), &buf);
		if (path.find(to_clear) != std::string::npos)
			path = path.substr(path.find(to_clear) + to_clear.size());
		*page << "<tr>" << std::endl;
		if (this->is_on(AUTOINDEXEXTEND) && indent)
			*page << "<td><a style=\"margin-left: " << indent << "px;\" href=\"" << path << "\">" << name << "</a></td>"<< std::endl;
		else
			*page << "<td><a style=\"margin-left: " << indent << "px;\" href=\"" << name << "\">" << name << "</a></td>"<< std::endl;
		*page << "<td>" << ctime(&buf.st_mtime) << "</td>" << std::endl;
		*page << "<td>" << buf.st_size << "</td>" << std::endl;
		*page << "</tr>" << std::endl;
	}
}
