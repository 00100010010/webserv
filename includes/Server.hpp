#ifndef SERVER_HPP
# define SERVER_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sstream>
#include <fstream>
#include <iomanip>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include <stdlib.h>

#include <iostream>
#include <vector>

#include <fcntl.h>

#include <ctime>
#include <sys/time.h>

#include <map>

#include "utils/msg_sys.hpp"
#include "Header.hpp"
#include "utils/colors.hpp"
#include <csignal>


#define CLTSTR		_clientFolder[i].recStr
#define CLTEND		_clientFolder[i].end
#define CLTTREC		_clientFolder[i].toRecv
#define CLTBODY		_clientFolder[i].body
#define CLTSTATE	_clientFolder[i].status
#define CLTBDSZ		_clientFolder[i].body_size
#define CLTSERVID	_clientFolder[i].serverId
#define CLTHEADER	_clientFolder[i].header
#define CLTCHUNK	_clientFolder[i].is_chunck
#define CLTTMPSCHU	_clientFolder[i].tmp_chunck
#define CLTSIZECHU	_clientFolder[i].size_chunck
#define CLTBUFFCHU	_clientFolder[i].buff_chunck

#define	CLTRESETTIMESTAMP gettimeofday(&_clientFolder[i].timestamp, NULL)
#define CLTGETTIMESTAMP _clientFolder[i].timestamp.tv_sec*1000LL + _clientFolder[i].timestamp.tv_usec/1000



#define SERVER_PORT(servId)			_config.getServerValue(servId, LISTEN, int())
#define BACKLOG(servId)				_config.getServerValue(servId, BACKLOG_VAL, int())
#define SERVER_IP(servId)			_config.getServerValue(servId, IP, std::string())
#define GET_MAX_BODY_SIZE(servId)	_config.getServerValue(servId, MAX_BODY_SIZE, int())
#define NB_BLOCK_SERV				_config.size()
#define TIMEOUT						_config.getValue("timeout" ,size_t())

#define SET_PARSE_VALUE(key, val)	_clientFolder[i].header.set_parsed_header_val(key, val)
#define GET_PARSE_VALUE(key)		_clientFolder[i].header.get_parsed_header_val(key)
#define COUNT_KEY(key)				_clientFolder[i].header.count_key(key)
#define CONTENT_LENGTH 		"Content-Length"
#define TRANSFER_ENCO		"Transfer-Encoding"
#define CONFIG_FILE 		"./config/config_1.conf"


#define BUFFER_SIZE 		8000

#define HEADER 0
#define BODY 1

void			signalHandler(int signum);
extern int		stopSignal;

struct ClientFolder{
	std::string		recStr;
	std::string		body;
	int 			end;
	unsigned long	toRecv;
	int				serverId;
	int 			status;
	int				is_chunck;
	size_t			tmp_chunck;
	size_t			size_chunck;
	size_t 			body_size;
	std::string		buff_chunck;
	Header			header;
	struct timeval timestamp; 

	ClientFolder(int id = -1) {
		end			= 0;
		recStr		= "";
		body		= "";
		toRecv		= 0;
		is_chunck	= 0;
		status		= HEADER;
		body_size	= 0;
		tmp_chunck	= 0;
		size_chunck = 0;
		serverId	= id;
		gettimeofday(&timestamp, NULL); 
	}

	~ClientFolder() {
	}
};

class Server {

	private:

		std::vector<int>					_sockVec;
		std::map<int, int>					_sockToConf;
		char								_buffer[BUFFER_SIZE];
		sockaddr_in							_sockIn;
		std::vector<ClientFolder>			_clientFolder;
		std::vector<pollfd>					_pollVec;
		// std::map<std::string, std::string>	_parsedHeader;
		int									_serverState;
		Config								_config;
		// Header								_header;
		Mime		const					_mime;
		MatchCode	const					_match_code;

	public:

		Server();
		Server(const char * config_file);
		~Server();

		void		requestGet(int);
		// std::string	generateHeader(std::string file);
		int			parseHeaderUtil(std::string &, size_t const, std::string const &, size_t, size_t);
		void		parseHeader(std::string header, size_t const i);
		void		parsebody(std::string body, size_t const i);
		void		display_header();
		int			generateServerSocket();
		int			acceptClientSocket(int & end_server);
		int			startListen();
		int			initPoll();
		void		interact();
		void		closeClientConnection(size_t);
		int			acceptConnection(int i);
		int			doRecv(size_t & i, bool header_mode);
		void		cleaner();
		int			dispatchMethod(size_t const);


		void 		requestPost(int i);
		void		requestDelete(int i);
		int			send_request(int i);

};
#endif
