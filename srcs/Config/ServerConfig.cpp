#include "ServerConfig.hpp"

///////////////////////////////////////////////////////////////////////////////
//										C O N S T R U C T O R S & destructor
///////////////////////////////////////////////////////////////////////////////
ServerConfig::ServerConfig() : _mainServerConfig(SubConfig(SERVER)) {
	_locationConfigVct.push_back(SubConfig(LOCATION));
}

ServerConfig::ServerConfig(const ServerConfig & x)
	:	_mainServerConfig	(x._mainServerConfig),
		_locationConfigVct	(x._locationConfigVct)
{}

ServerConfig::~ServerConfig() {}

///////////////////////////////////////////////////////////////////////////////
//												relationnal O P E R A T O R S
///////////////////////////////////////////////////////////////////////////////
ServerConfig & ServerConfig::operator = (const ServerConfig & x) {
	_mainServerConfig 	= x._mainServerConfig;
	_locationConfigVct	= x._locationConfigVct;
	return *this;
};

///////////////////////////////////////////////////////////////////////////////
//										public P A R S I N G  F U N C T I O N S
///////////////////////////////////////////////////////////////////////////////
void ServerConfig::addLocation() {
	_locationConfigVct.push_back(SubConfig(LOCATION));
}

int ServerConfig::addLine(std::string const & line, int flag)
{
	if (flag)
		return _locationConfigVct[size() - 1].addLine(line, false);
	else
		return _mainServerConfig.addLine(line, false);
}

///////////////////////////////////////////////////////////////////////////////
//																S E T T E R S
///////////////////////////////////////////////////////////////////////////////
void ServerConfig::setNameLastLocation(std::string const & line) {
	std::string name = extractLocationName(line);
	_locationConfigVct[size() - 1].setLocationName(name);
}

///////////////////////////////////////////////////////////////////////////////
//																G E T T E R S
///////////////////////////////////////////////////////////////////////////////
int	ServerConfig::getmatchLocation(url_extract const & url_struct) const {
	std::string line, path, ext = url_struct._ext;
	if (ext.size() && ext[0] == '.')
		ext.erase(0, 1);
	std::map<size_t, int> priority_queue;
	for (int i = 0; i < static_cast<int>(size()); i++) {
		line = _locationConfigVct[i].getName();
		if (line.find('(') == std::string::npos) {
			if (url_struct._folder + url_struct._file_name == line)
				return i;

			if (url_struct._folder.find(line) != std::string::npos)
				priority_queue[2] = i;
		}
		else {
			if (!ext.empty() && line.find(ext) != std::string::npos) {
				priority_queue[3] = i;
			}
		}
	}
	if (priority_queue.size() == 0)
		return -1;
	return priority_queue.begin()->second;
}

const SubConfig * ServerConfig::getLocation(size_t n) const {
	if (n >= 0 && n < size())
		return &_locationConfigVct[n];
	return NULL;
}

size_t	ServerConfig::size() const {
	return _locationConfigVct.size();
}

const SubConfig * ServerConfig::getServerConfig() const {
	return &_mainServerConfig;
}

bool	ServerConfig::isMethodAllowed(std::string const & method) const {
	if (	method != "POST"
		&&	method != "GET"
		&&	method != "DELETE")
		return false;
	std::string line = getValue("methods", std::string());
	if (line.find(method) != std::string::npos)
		return true;
	return false;
}
///////////////////////////////////////////////////////////////////////////////
//																D I S P L A Y
///////////////////////////////////////////////////////////////////////////////
void ServerConfig::display_log(bool verbose) const {
	_mainServerConfig.display_log(verbose);
	std::string sep1	= "────────┴───────┐";
	std::string sep2	= "────────────────┤";
	std::string sep		= sep1;

	for (std::vector<SubConfig>::const_iterator it = _locationConfigVct.begin()
		; it != _locationConfigVct.end(); it++) {
		if (verbose)
			std::cout << "|";
		std::cout 	<< "       ├─── " << RED "LOCATION"
					<< RESETCOLOR " ────[ " RED << std::setw(20)
					<< std::setfill('_') << it->getName() << " " RESETCOLOR
					<< "]";
		if (verbose)
			std::cout << sep << std::setfill(' ');
		std::cout << std::endl;
		sep = sep2;
		it->display_log(verbose);
	}
}

