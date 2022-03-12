#include "SubConfig.hpp"

///////////////////////////////////////////////////////////////////////////////
//										C O N S T R U C T O R S & destructor
///////////////////////////////////////////////////////////////////////////////
SubConfig::SubConfig(int const mode) : _mode(mode) {
	if (mode == LOCATION) {
		initTemplate(TEMPLATE_LOCATION);
		setLocationName("DEFAULT");
	}
	else if (mode == MAIN)
		initTemplate(TEMPLATE_MAIN);
	else
		initTemplate(TEMPLATE_MAIN_SERV);
}

SubConfig::SubConfig(const SubConfig & x)
	:	_configMap		(x._configMap),
		_mode			(x._mode),
		_locationName	(x._locationName)
{}

SubConfig::~SubConfig() {}

///////////////////////////////////////////////////////////////////////////////
//												relationnal O P E R A T O R S
///////////////////////////////////////////////////////////////////////////////
SubConfig &	SubConfig::operator = (const SubConfig & x) {
	_configMap		= x._configMap;
	_mode			= x._mode;
	_locationName	= x._locationName;
	return *this;
};

///////////////////////////////////////////////////////////////////////////////
//										private P A R S I N G  F U N C T I O N S
///////////////////////////////////////////////////////////////////////////////
int SubConfig::initTemplate(const char * filename) {
	std::ifstream file;
	if (openFile(file, filename) == -1)
		return -1;

	std::string line;
	while (std::getline(file, line)) {
		truncSpaces(line);
		if (line.empty() || isCommentLine(line))
			continue;

		if (addLine(line, true) == -1)
			break ;
	}
	file.close();
	return 0;
}

static bool isCgiPath(std::string const & path) {
	if (path.empty() || path.find(" \t\n\r\v\f") != std::string::npos)
		return false;
	if (path.size() == 1 && path[0] == '\\')
		return false;
	return true;
}

static bool isCgiExt(std::string const & ext) {
	if (ext.empty())
		return false;
	size_t x = ext.find(" \t\n\r\v\f");
	if (x != std::string::npos) {
		if (x == 0)
			return false;
		if (ext[x - 1] != '\\')
			return false;
	}
	return true;
}

static std::pair<bool, std::string>	formatCgi(std::string const & line) {
	std::string cgi_format;
	if (line != "default") {
		if (line.find(' ') == std::string::npos)
			return std::make_pair(false, "error");
		std::string ext = line.substr(0, line.find(' '));
		if (ext.size() + 1 >= line.size())
			return std::make_pair(false, "error");
		std::string path = line.substr(ext.size() + 1, line.size() - ext.size() + 1);
		truncSpaces(ext); truncSpaces(path);
		if (!isCgiExt(ext) || !isCgiPath(path))
			return std::make_pair(false, "error");
		cgi_format = ext + " " + path;
	}
	else
		cgi_format = line;
	return std::make_pair(true, cgi_format);
}

void	SubConfig::add_cgi(std::string const & line) {
	if (line == "default") {
		if (_configMap["cgi"] == line || !_configMap["cgi"].empty())
			return ;
	}

	if (formatCgi(line).first == false)
		return;
	std::string cgi_format = formatCgi(line).second;

	if (_configMap["cgi"].empty() || _configMap["cgi"] == "default")
		_configMap["cgi"] = cgi_format;
	else {
		std::string cgi_bckp = _configMap["cgi"];
		cgi_bckp.append(" | " + cgi_format);
		_configMap["cgi"] = cgi_bckp;
	}
}


static std::map<std::string, std::string> getMapCgi(std::string const & line) {
	std::vector<std::string> v_line = ft_split2(line, "|");
	for (size_t i = 0; i < v_line.size(); i++)
		truncSpaces(v_line[i]);
	std::map<std::string, std::string> map_cgi;
	std::string ext, path;
	for (size_t i = 0; i < v_line.size(); i++) {
		ext	 = v_line[i].substr(0, v_line[i].find(' '));
		if (map_cgi[ext].size())
			continue;
		path = v_line[i].substr(ext.size() + 1, v_line[i].size() - ext.size() + 1); 
		map_cgi[ext] = path;
	}
	return map_cgi;
}

///////////////////////////////////////////////////////////////////////////////
//										public P A R S I N G  F U N C T I O N S
///////////////////////////////////////////////////////////////////////////////

int SubConfig::addLine(std::string const & line, bool const initTemplate) {
	if (line.empty())
		return 0;
	size_t x = line.find_first_of(SEP);
	if (x == std::string::npos) {
		INFO_SYST("Error: Config file: in line : ["
				<< line << "]", "missing separator !!")
		return -1;
	}
	std::string code, instruct;
	code    .assign(line.begin(), line.begin() + x);
	instruct.assign(line.begin() + code.size() + SEP.size(), line.end());
	truncSpaces(code); truncSpaces(instruct);
	if (instruct.empty()) {
		if (initTemplate == true) {
			INFO_SYST("empty instruction in template file code : ["
						<< code  << "]", "")
			return -1;
		}
		return 0;
	}
	if (initTemplate == false && _configMap.find(code) == _configMap.end()) {
		INFO_SYST("Invalid code in SubConfig file : ["
					<< code << "]", "doesn't exists.")
		return 0;
	}
	if (code != "cgi" && checker(code, instruct) == 0)
		_configMap[code] = instruct;
	else if (code == "cgi")
		add_cgi(instruct);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//																S E T T E R S
///////////////////////////////////////////////////////////////////////////////
void SubConfig::setLocationName(std::string const & name) {
	_locationName = name;
}

///////////////////////////////////////////////////////////////////////////////
//																G E T T E R S
///////////////////////////////////////////////////////////////////////////////
std::string	const &	SubConfig::getName() const {
	return _locationName; }

std::string const & SubConfig::getValue(std::string const & key, std::string type) const {
	(void)type;
	return _configMap.find(key)->second;
}

int SubConfig::getValue(std::string const & key, int type) const {
	(void)type;
	return  std::atoi(_configMap.find(key)->second.c_str());
}

float SubConfig::getValue(std::string const & key, float type) const {
	(void)type;
	return  std::atof(_configMap.find(key)->second.c_str());
}

size_t SubConfig::getValue(std::string const & key, size_t type) const {
	(void)type;
	return  std::strtoul(_configMap.find(key)->second.c_str(), NULL, 11);
}

bool	SubConfig::isMethodAllowed(std::string const & method) const {
	if (	method != "POST"
		&&	method != "GET"
		&&	method != "DELETE")
		return false;
	std::string line = getValue("methods", std::string());
	if (line.find(method) != std::string::npos)
		return true;
	return false;
}
// 0 
// 1 Folder
// 2 File
// 3 Extension
int	SubConfig::whatLocationType() const {
	if (_mode != 1 || getName().empty())
		return Null;
	std::string loc = getName();
	if (loc[0] == '/' && loc[loc.size() - 1] == '/')
		return Folder;
	else if (loc[0] == '(' && loc[loc.size() - 1] == ')')
		return Extension;
	else
		return File;
}

std::pair<bool, std::string> SubConfig::getMatchCgi(std::string const & ext) const {
	if (ext.empty() || _mode != 2 || getValue("cgi", std::string()) == "default")
		return std::make_pair(0, "");
	std::map<std::string, std::string> map_cgi = getMapCgi(getValue("cgi", std::string()));
	if (map_cgi[ext].size())
		return std::make_pair(1, map_cgi[ext]);
	return std::make_pair(0, "");
}

///////////////////////////////////////////////////////////////////////////////
//																D I S P L A Y
///////////////////////////////////////////////////////////////////////////////
void SubConfig::display_log(bool verbose) const {
	std::map<std::string, std::string>::const_iterator it = _configMap.begin();
	SubConfig tmp(_mode);

	for ( ; it != _configMap.end(); it++) {
		if (!verbose && tmp.getValue(it->first, std::string()) == it->second)
			continue;
		if (verbose)
			std::cout << RESETCOLOR "│";
		if (_mode == LOCATION)
			std::cout << RESETCOLOR "       │       ";
		else if (_mode == SERVER)
			std::cout << RESETCOLOR "       ";

		std::cout	<< std::setfill(' ') << "│ " GRN << std::right << std::setw(18) << it->first
					<< RESETCOLOR << std::left << " │ " CYN;
		if (verbose)
			std::cout << std::setw(27);
		std::cout << it->second << RESETCOLOR;
		if (verbose)
			std::cout << "│";
		std::cout << std::endl;
	}
}

