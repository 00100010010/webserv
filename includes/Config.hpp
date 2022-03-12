#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "ServerConfig.hpp"

/*

___ CONFIG
	(+ SubConfig mode GLOBAL)
	|________SERVERCONFIG
			(+ SubConfig mode SERVER)
			|_______LOCATION CONFIG (is SubConfig mode LOCATION)

*/
#define NOTFOUND			"404"
#define BADREQUEST			"400"
#define FORBIDEN			"403"

#define ROOT				"root"
#define DEFAULT_PAGE		"index"
#define LISTEN				"listen"
#define SERVER_NAME			"server_name"
#define BACKLOG_VAL			"backlog"
#define IP					"ip"
#define MAX_BODY_SIZE		"max_body_size"
#define AUTOINDEX			"autoindex"
#define AUTOINDEXEXTEND 	"autoindexextend"
#define REWRITE			 	"rewrite"
#define ALL_DOWNLOAD		"upload"
#define DEFAULT_VAL		 	"default"
#define DOWNLOAD			"501_on"
#define CGITEMOUT			"cgi_timeout"
#define VERSION_HTTP		"HTTP/1.1"

#define WEBSERV_VERSION		"webserv/1.0.42 (42 geng)"

#define CONFIGFILE_LOG		"./config.log"


class Config
{

///////////////////////////////////////////////////////////////////////////////
//															A T T R I B U T E S
///////////////////////////////////////////////////////////////////////////////
	private:

		SubConfig					_mainConfig;
		std::vector<ServerConfig>	_serverConfigVct;

///////////////////////////////////////////////////////////////////////////////
//									private P A R S I N G  F U N C T I O N S
///////////////////////////////////////////////////////////////////////////////
	private:

		int	parseFile(const char * filename);
		int	addLine(std::string const &, size_t const,
													size_t const, size_t const);




	public:

///////////////////////////////////////////////////////////////////////////////
//									C O N S T R U C T O R S & destructor
///////////////////////////////////////////////////////////////////////////////
			Config();
			Config(const char * filename);
			Config(const Config & x);
			~Config();

///////////////////////////////////////////////////////////////////////////////
//												relationnal O P E R A T O R S
///////////////////////////////////////////////////////////////////////////////
		Config &	operator = (const Config & x);

///////////////////////////////////////////////////////////////////////////////
//																G E T T E R S
///////////////////////////////////////////////////////////////////////////////
		void								displayPorts(Config const & )	const;
		ServerConfig				const &	getServerConfig	(size_t const n)const;
		std::vector<ServerConfig>	const & getServerConfig()				const;
		size_t								size()							const;
///////////////////////////////////////////////////////////////////////////////
//						G E T T E R S to locations params (or server if default)
///////////////////////////////////////////////////////////////////////////////
// 1st param = server num
// 2nd param = location (via size_t or string name)
// 3rd param is param key
// -> returns server param if default location param
		template <typename X>
		X getLocationValue(size_t serv, int locID, std::string const & key, X type = X()) const
		{
			return _serverConfigVct[serv].getLocationValue(locID, key, type);
		};

///////////////////////////////////////////////////////////////////////////////
//												G E T T E R S to server params
///////////////////////////////////////////////////////////////////////////////
		template <typename X>
		X getServerValue(size_t serv, std::string const & key, X type = X()) const
		{
			return _serverConfigVct[serv].getValue(key, type);
		};

		template <typename X>
		X getValue(std::string const & key, X type = X()) const
		{
			return _mainConfig.getValue(key, type);
		};

///////////////////////////////////////////////////////////////////////////////
//																D I S P L A Y
///////////////////////////////////////////////////////////////////////////////
		void				display_log(bool verbose = 0)				const;
		void				displayFile()								const;

};

#endif
