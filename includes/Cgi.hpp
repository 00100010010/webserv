

#include "utils.hpp"
#include "utils/msg_sys.hpp"
#include "ServerConfig.hpp"
#include "Config.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/time.h>

#define SUB_ID					this->_subconfig_id
#define CONFIG(key, T)			this->_config->getValue(key, T)
#define MY_CONFIG(key)			this->get_config_val(key)
#define SUBCONFIG(key, T)		this->_config->getLocationValue(this->_subconfig_id, key, T)
#define PAIR_STR_INT			std::pair<std::string, int>
#define PAIR_STR_STR			std::pair<std::string, std::string>

#define GSETTIME gettimeofday(&te, NULL);
#define GGETTIME te.tv_sec*1000LL + te.tv_usec/1000
#define GETCGITIME time.tv_sec*1000LL + time.tv_usec/1000
class Cgi {
	public:
		Cgi();
		Cgi(Cgi const & src);
		Cgi(MAP_STRING header, ServerConfig const * config, int subconfig, url_extract data);
		~Cgi() ;

		Cgi & operator=(Cgi const &src);

		void			set_url(url_extract data);
		void			set_header(MAP_STRING header);
		void			set_servConfig(ServerConfig const * config);
		void			set_id_sub(int sub);

		PAIR_STR_INT	get_cgi_body(std::string data);
		std::string		get_config_val(std::string key);

		void			convert_env_cgi();

		void			set_cgi_env();
		pid_t				process_cgi(std::string data);
		PAIR_STR_INT	get_cgi_response(pid_t pid, struct timeval time);
		std::string		get_parsed_header_val(std::string key);

		PAIR_STR_STR		process_body(std::string val);

		void			sys_call();

		void			clear();

	private:
		MAP_STRING				_parsed_header;
		ServerConfig const *	_config;
		int						_subconfig_id;
		MAP_STRING				_cgi_env;


		url_extract				_data;

		pid_t					_pid;
		// int						pipe_fd[2];
		int						out_fd[2];
		int						_out_fd;

		size_t					_wait_time;
		struct timeval tv;
		char**					_env_val;
		char**					_arg_val;
		std::string				_query_string;
};
