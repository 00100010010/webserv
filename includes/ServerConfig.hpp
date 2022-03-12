#ifndef SERVCONF_HPP
# define SERVCONF_HPP

#include "SubConfig.hpp"


// #define DBG(x)			if (FLAG) std::cout << RED << "[DBG] : " << x << RESETCOLOR << std::endl;

#define FILE_NAME				this->_data._file_name
#define FILE_FOLDER				this->_data._folder
#define FILE_PATH				this->_data._path
#define FILE_EXT				this->_data._ext
#define TMP_FILE				this->_data._tmp_file
#define AUTO_INDEX				this->_data._auto_x

struct url_extract {
	std::string		_file_name;
	std::string		_folder;
	std::string		_path;
	std::string		_ext;
	std::string		_tmp_file;
	int				_auto_x;
};

class ServerConfig {

///////////////////////////////////////////////////////////////////////////////
//															A T T R I B U T E S
///////////////////////////////////////////////////////////////////////////////
	private:

		SubConfig					_mainServerConfig;
		std::vector<SubConfig>		_locationConfigVct;





	public:

///////////////////////////////////////////////////////////////////////////////
//									C O N S T R U C T O R S & destructor
///////////////////////////////////////////////////////////////////////////////
							ServerConfig();
							ServerConfig	(const ServerConfig &);
							~ServerConfig();

///////////////////////////////////////////////////////////////////////////////
//												relationnal O P E R A T O R S
///////////////////////////////////////////////////////////////////////////////
		ServerConfig &		operator = (const ServerConfig &);

///////////////////////////////////////////////////////////////////////////////
//										public P A R S I N G  F U N C T I O N S
///////////////////////////////////////////////////////////////////////////////
		void				addLocation();
		int					addLine(std::string const &, int);

///////////////////////////////////////////////////////////////////////////////
//																S E T T E R S
///////////////////////////////////////////////////////////////////////////////
		void 				setNameLastLocation(std::string const &);

///////////////////////////////////////////////////////////////////////////////
//																G E T T E R S
///////////////////////////////////////////////////////////////////////////////
		size_t				size			()								const;
		const SubConfig *	getLocation		(size_t)						const;
		const SubConfig *	getServerConfig	()								const;
		bool				isMethodAllowed	(const std::string &)			const;
		bool				isMethodAllowed	(const std::string &, const std::string &)const;
		int					getmatchLocation(url_extract const & url_struct)const;

///////////////////////////////////////////////////////////////////////////////
//					G E T T E R S to locations params (or server if default)
///////////////////////////////////////////////////////////////////////////////
// 1st param = location (via size_t or string name)
// 2nd param is param key
// returns server param if default location param
		template <typename X>
		X getLocationValue(int locID, std::string const & key, X type = X()) const {
			if (locID == -1)
				return getValue(key, type);

			const SubConfig * location = getLocation(locID);
			SubConfig tmp(LOCATION);

			if (key != "cgi" && location->getValue(key, type) == tmp.getValue(key, type))
				return getValue(key, type);

			return location->getValue(key, type);
		};

///////////////////////////////////////////////////////////////////////////////
//												G E T T E R S to server params
///////////////////////////////////////////////////////////////////////////////
		template <typename X>
		X getValue(std::string const & key, X type = X()) const {
			return _mainServerConfig.getValue(key, type);
		};
///////////////////////////////////////////////////////////////////////////////
//																D I S P L A Y
///////////////////////////////////////////////////////////////////////////////
		void				display_log(bool verbose = 0)	const;

};

#endif
