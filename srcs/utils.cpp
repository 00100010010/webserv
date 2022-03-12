#include "utils.hpp"

unsigned long ato_octets(std::string const & line)
{
	std::string value = line;
	eraseSpaces(value);
	for (size_t i = 0; i < value.size() ; i++)
		value[i] = std::tolower(value[i]);

	if (value.find_first_not_of("0123456789gmko")	!= std::string::npos
		|| value.find_first_of("0123456789")		== std::string::npos
		|| value.find_first_of("gmko")				== std::string::npos)
		return 0;

	size_t x			= value.find_first_of("gmko");
	std::string type    = value.substr(x, value.size() - x);
	value               = value.substr(0, x);

	if (type != "o" && type != "ko" && type != "mo" && type != "go")
		return 0;

	x = 0;
	switch (type[0]) {
		case 'g':
			x += 3;
		case 'm':
			x += 3;
		case 'k':
			x += 3;
		case 'o':
			return std::strtoul(value.c_str(), NULL, 0) * std::pow(10, x);
		default:
			return 0;
	}
}

std::vector<std::string>	split_NoEscape(std::string const & line_origin,
										std::string const & delimiter)
{
	std::vector<std::string>	vct;
	std::string					line	= line_origin;
	size_t						x		= 0;

	for (size_t i = 0; i < line.size() && line[i] == '\\' ; i++)
		line.erase(0, 1);
	if (line[0] == '/')
		line.erase(0, 1);
	if (line[line.size() - 1] != '/')
		line.push_back('/');

	while (((x = line.find(delimiter))) != std::string::npos)
	{
			vct.push_back(line.substr(0, x));
			line.erase(0, x + 1);
	}
	return vct;
}

bool	isTryingtoEscape(std::string const & path)
{
	if (path.empty())
		return false;
	int							hack_meter	= 0; 
	std::vector<std::string>	vct			= split_NoEscape(path, "/");

	for (std::vector<std::string>::iterator it = vct.begin(); it != vct.end(); it++)
	{
		if (*it == "..")
			hack_meter--;
		else if (*it != ".")
			hack_meter++;
		if (hack_meter < 0)
			break ;
		if ((*it).size() && (*it).at((*it).size() - 1) == '\\')
			*it++;
	}
	return (hack_meter >= 0) ? false : true;
}

std::vector<std::string> ft_split(std::string const & line_origin,
									std::string const & delimiter) {
	std::vector<std::string>	vct;
	std::string					line	= line_origin;
	size_t						x		= 0;
	while (((x = line.find(delimiter, x))) != std::string::npos) {
		if (x && line[x - 1] == '\\') {
			x++;
			continue;
		}
		vct.push_back(line.substr(0, x));
		line.erase(0, x + 1);
		x = 0;
	}
	if (!line.empty())
		vct.push_back(line);
	return vct;
}

static int	extractRule(std::string const & full_line, std::string & to_search,
										std::string & new_url, size_t & code) {
	std::vector<std::string> vct = ft_split(full_line, " ");
	if (vct.size() != 3)
		return -1;

	to_search	= vct[0];
	new_url		= vct[1];

	for (size_t i = 0; i < vct[2].size(); i++) {
		if (!std::isdigit(vct[2][i]))
			return -1;
	}
	code = std::atol(vct[2].c_str());
	return 0;
}

static int	formatOutput(std::string & url, std::string const & to_search,
												std::string & new_url) {
	if (new_url.find("^") == std::string::npos)
		return 0;
	size_t flag = 0;
	if (new_url[0] == '^') {
		new_url.erase(0, 1);
		flag = 1;
	}
	if (new_url[new_url.size() - 1] == '^') {
		new_url.erase(new_url.size() - 1, 1);
		flag = flag ? 3 : 2;
	}
	// .find() already secured in fct checkSimili()
	size_t x = url.find(to_search);

	if (flag == 1)
		url.erase(x + to_search.size(), url.size() - x - to_search.size());
	else if (flag == 2)
		url.erase(0, x);

	x		= url.find(to_search);
	new_url	= url.replace(x, to_search.size(), new_url);
	return 0;
}

//	 /tata/		flag = 0
//	*/tata/		flag = 1
//	 /tata/*	flag = 2
//	*/tata/*	flag = 3
static bool checkSimili(std::string const & url, std::string & to_search) {
	size_t flag	= 0;
	if (to_search[0] == '*') {
		to_search.erase(0, 1);
		flag = 1;
	}
	if (to_search[to_search.size() - 1] == '*') {
		to_search.erase(to_search.size() - 1, 1);
		flag = flag ? 3 : 2;
	}

	if (url.find(to_search) == std::string::npos)
		return false;
	if (flag == 0 && url != to_search)
		return false;

	size_t x = url.find(to_search);
	if	((  flag == 1 && x == 0)
		|| (flag == 2 &&  url.size() - x == to_search.size())
		|| (flag == 3 && (url.size() - x == to_search.size() || x == 0)))
		return false;
	return true;
}

std::pair<std::string, size_t> check_redirection(std::string const & url_origin,
											std::string const & full_line) {
	std::string url = url_origin, to_search, new_url;
	size_t code;

	if (extractRule(full_line, to_search, new_url, code) == -1) {
		std::cerr
		<<"error: utils.cpp: check_redirection: parsing:extractRule"
		<< std::endl;
		return std::pair<std::string, size_t>("", 0);
	}

	if (!checkSimili(url_origin, to_search)) {
		return std::pair<std::string, size_t>("", 0);
	}

	if (formatOutput(url, to_search, new_url) == -1) {
		return std::pair<std::string, size_t>("", 0);
	}

	return std::pair<std::string, size_t>(new_url, code);
}

bool isValidHTTPversion(std::string const & value) {
	if (value != "HTTP/1.1")
		return 0;
	return 1;
}

size_t endofSpaces(std::string const & line, size_t x) {
	size_t i = x;
	for (; i < line.size() && (line[i] == ' ' || line[i] == '\t');) {
		i++;
	}
	return i;
}

bool	isValidHost(std::string const & host) {
	if (host.find('@') != NPOS || host.empty()) {
		return false;
	}
	std::string line = host;
	truncSpaces(line);
	if (line.empty())
		return false;
	return true;
}

bool	isMethodAllowed(std::string const & method1,
						std::string const & method2) {
	if (	method1 != "POST"
		&&	method1 != "GET"
		&&	method1 != "DELETE")
		return false;
	if (method2.find(method1) != std::string::npos)
		return true;
	return false;
}
