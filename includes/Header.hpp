/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ifaveur <ifaveur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 11:35:58 by ifaveur           #+#    #+#             */
/*   Updated: 2021/11/30 17:05:45 by ifaveur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "Config.hpp"
#include "Cgi.hpp"

#include <sys/socket.h>

#include <iostream>
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>

#ifdef	__linux__
#	define LINUX 1
#else
#	define LINUX 0
#endif

#define HEADER_LOG_FILE	"./headers.log"


#define VEC_MAP_STR		std::vector<MAP_STRING>

#define	SETCGITIME gettimeofday(&_timestamp, NULL)

#define READ_ERROR	1
#define READ_OK		0

// taking value of a server, and not a location
//(getLocationValue(size_t servernumber, size_t locationnumber(or string name/title location), string key, type(/string()/int()/size_t()/float())))
// #define CONFIG(key, T)			this->_config->getValue(key, T)
// #define MY_CONFIG(key)			this->get_config_val(key)
// #define SUBCONFIG(key, T)		this->_config->getLocationValue(this->_subconfig_id, key, T)
#define GET_ERROR(code)			this->_match_code.get_message(code)
#define GET_MIME(ext)			this->_mime_list.get_mime(ext);

// #define SUB_ID					this->_subconfig_id



// 404 ERROR PAGE
// 400 ERROR PAGE
// ERROR page can be generate if not defined in config files

class Header {
	private:
		MAP_STRING		_parsed_header;

		Mime					_mime_list;
		MatchCode				_match_code;
		Cgi						_cgi;

		ServerConfig  		const	*_config;
		ServerConfig 		const	*_tmp_config;
		Config				const 	*_conf;

		int fd;

		pid_t _pid_cgi;
		size_t _time;
		struct timeval _timestamp; 

		url_extract		_data;
		int				_subconfig_id;
		std::string		_query_string;
		std::string		_mime;
		std::string		_status;
		std::string		_file_buffer;
		int				_read;
		int 			_code;
		int				_send;

		std::string		_header;
		std::string		_body;
		size_t			size_send;
		int				step;

	public:

		Header();
		~Header();

		ServerConfig  const * 		get_server_config()const {return _config;};
		int							init();
		void						reset_value();

		std::string					generate_tmp_file();

		void						set_parsed_header_val(std::string key, std::string val);
		std::string					get_parsed_header_val(std::string key);
		void						set_send_fd(int x);

		void						identify_subconfig();
		void						set_server_config(ServerConfig const * config);
		void 						set_config(Config const *conf);
		size_t						get_int_config_value(std::string key);
		std::string					get_config_val(std::string key);
		size_t						count_key(std::string key);

		int 						generate_get_response();
		int 						post_request();
		int							generate_error_response(int code);
		int							redirection(std::pair<std::string, size_t> location);
		int							post_extend();
		int 						delete_request();

		// int							send_to_client(std::string str, int body = 0);
		int							write_request();

		int							parse_body_request();
		std::vector<std::string>	split(std::string val);
		void						upload(std::string name, std::string content);

		void 						set_extension();
		void						set_mime();
		void						set_file_name();
		void						set_path();

		int							check_file_name();

		void						display_parsed()	const;
		void						display_header()	const;

		std::string					clear_path(std::string path);

		bool						is_on(std::string key);
		void						auto_index();
		void						folder_content(std::stringstream *page, std::string path, int indent, std::string name);

		std::string					generete_error_page(int code, std::string message);
		int							set_error(int code, int x = 0);
		int							set_file_content(int x = 0);

		int							is_cgi_on;
		int							to_send;
		int							is_finished;
		int							err_code;
};
