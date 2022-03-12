#include "Config.hpp"

///////////////////////////////////////////////////////////////////////////////
//									C O N S T R U C T O R S & destructor
///////////////////////////////////////////////////////////////////////////////
Config::Config() : _mainConfig(SubConfig(MAIN)) {
	_serverConfigVct.push_back(ServerConfig());
}

Config::Config(const char * filename) : _mainConfig(SubConfig(MAIN)) {
	_serverConfigVct.push_back(ServerConfig());
	parseFile(filename);
}

Config::Config(const Config & x)
:	_mainConfig		(x._mainConfig),
	_serverConfigVct(x._serverConfigVct)
{}

Config::~Config() {}

///////////////////////////////////////////////////////////////////////////////
//												relationnal O P E R A T O R S
///////////////////////////////////////////////////////////////////////////////
Config & Config::operator = (const Config & x) {
	_mainConfig			= x._mainConfig;
	_serverConfigVct	= x._serverConfigVct;
	return *this;
};

///////////////////////////////////////////////////////////////////////////////
//									private P A R S I N G  F U N C T I O N S
///////////////////////////////////////////////////////////////////////////////
int Config::addLine(std::string const & line, size_t const flagLoc,
									size_t const flagServ, size_t const i) {
	if (flagServ % 2 == 0)
		return _mainConfig.addLine(line, false);
	else if (flagServ)
		return _serverConfigVct[i].addLine(line, flagLoc % 2);
	return -1;
}

int Config::parseFile(const char * filename) {
	std::ifstream file;
	if (openFile(file, filename) == -1)
		return -1;

	size_t		flagServ = 0, flagLoc = 0;
	int			i = -1;
	std::string	line;
	while (std::getline(file, line)) {
		truncSpaces(line);
		if (line.empty() || isCommentLine(line))
			continue;

		if (isNextServ(line)) {
			if (!isLastPartClosed(flagServ, filename, "server"))
				break ;

			if (i >= 0)
				_serverConfigVct.push_back(ServerConfig());
			flagServ++;
			flagLoc = 0;
			i++;
		}
		else if (isNextLocation(line)) {
			if (!isLastPartClosed(flagLoc, filename, "location"))
				break ;
			if (flagLoc > 0)
				_serverConfigVct[i].addLocation();
			_serverConfigVct[i].setNameLastLocation(line);
			flagLoc++;
		}
		else if (isEndPart(line)) {
			if (flagLoc % 2 != 0)
				flagLoc++;
			else if (flagServ % 2 != 0)
				flagServ++;
		}
		else {
			if (addLine(line, flagLoc, flagServ, i) == -1)
				break ;
		}
	}
	file.close();
	if (!isLastPartClosed(flagServ, filename, "server"))
		return -1;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//																G E T T E R S
///////////////////////////////////////////////////////////////////////////////

ServerConfig const & Config::getServerConfig(size_t n) const {
	return _serverConfigVct[n];
}

std::vector<ServerConfig> const & Config::getServerConfig() const {
	return _serverConfigVct;
}

size_t Config::size() const {
	return _serverConfigVct.size();
}
///////////////////////////////////////////////////////////////////////////////
//																D I S P L A Y
///////////////////////////////////////////////////////////////////////////////
void	Config::displayPorts(Config const & config) const {
	std::cout << CYN "AVALAIBLE PORTS :" << std::endl;
	for (size_t i = 0; i < config.size(); i++) {
		MSG_SYST("port : [" << YEL << config.getServerValue(i, "listen", std::string())<< GRN << "]")
	}
	std::cout << std::endl;
}

void Config::display_log(bool verbose) const {
	std::string	sep;
	std::string	startSpace;
	if (verbose)
	{
		startSpace	= "│       ";
		sep			= "────────────────────────────────────┐";
	}
	else
	{
		startSpace	= "       ";
		sep			= "";
	}
	if (verbose)
		std::cout	<< std::endl << RED "GLOBAL" RESETCOLOR "────────"
					<< sep << std::endl;
	_mainConfig.display_log(verbose);
	int i = 0;
	for (std::vector<ServerConfig>::const_iterator it = _serverConfigVct.begin()
		; it != _serverConfigVct.end(); it++) {
		std::cout 	<< startSpace << RED "SERVER" RESETCOLOR " [ " RED
					<< std::right << std::setw(3) << i << RESETCOLOR
					<< std::setfill(' ')<< " ]" << sep << std::endl;

		it->display_log(verbose);
		i++;
		if (!verbose)
			continue ;
		std::cout << startSpace
		<< "└───────┴─────────────────────────────────────────────────┘"
		<< std::endl;
	}
	if (!verbose) {
		std::cout << std::endl;
		return ;
	}
	std::cout
	<< "└─────── ─ ─ ─ ─ ─ ─ ─ ─  ─  ─  ─   ─   ─   ─    ─     -     -      "
	<< std::endl;
}

void Config::displayFile() const {

    std::streambuf *coutbuf = std::cout.rdbuf();
    std::ofstream outlog(CONFIGFILE_LOG);
    std::cout.rdbuf(outlog.rdbuf());

	display_log(1);

    std::cout.rdbuf(coutbuf);
}