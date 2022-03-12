/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ifaveur <ifaveur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/09 08:49:17 by ifaveur           #+#    #+#             */
/*   Updated: 2021/11/26 14:39:11 by ifaveur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <map>
#include <cmath>
#include <vector>
#include "utilsConfig.hpp"

#define MAP_STRING			std::map<std::string, std::string>
#define MAP_INT_STRING		std::map<int, std::string>
#define NPOS std::string::npos

class Mime {
	private:
		MAP_STRING _val;

	public:
		Mime() {
			this->_val["atom"]		= "application/atom+xml";
			this->_val["iges"]		= "application/iges";
			this->_val["js"]		= "application/javascript";
			this->_val["dxf"]		= "application/dxf";
			this->_val["mp4"]		= "application/mp4";
			this->_val["iges"]		= "application/iges";
			this->_val["igs"]		= "application/iges";
			this->_val["bin"]		= "application/octet-stream";
			this->_val["doc"]		= "application/msword";
			this->_val["pdf"]		= "application/pdf";
			this->_val["ps"]		= "application/postscript";
			this->_val["eps"]		= "application/postscript";
			this->_val["ai"]		= "application/postscript";
			this->_val["rtf"]		= "application/rtf";
			this->_val["sgml"]		= "application/sgml";
			this->_val["xls"]		= "application/vnd.ms-excel";
			this->_val["ppt"]		= "application/vnd.ms-powerpoint";
			this->_val["xml"]		= "application/xml";
			this->_val["tar"]		= "application/x-tar";
			this->_val["man"]		= "application/zip";
			this->_val["au"]		= "audio/basic";
			this->_val["snd"]		= "audio/basic";
			this->_val["mpg"]		= "audio/mpeg";
			this->_val["mp3"]		= "audio/mpeg";
			this->_val["mp4"]		= "audio/mp4";
			this->_val["aifc"]		= "audio/x-aiff";
			this->_val["aiff"]		= "audio/x-aiff";
			this->_val["aif"]		= "audio/x-aiff";
			this->_val["wav"]		= "audio/x-wav";
			this->_val["man"]		= "image/gif";
			this->_val["jpeg"]		= "image/jpeg";
			this->_val["jpg"]		= "image/jpeg";
			this->_val["jpeg"]		= "image/jpeg";
			this->_val["ico"]		= "image/ico";
			this->_val["png"]		= "image/png";
			this->_val["tiff"]		= "image/tiff";
			this->_val["tif"]		= "image/tiff";
			this->_val["pbm"]		= "image/x-portable-bitmap";
			this->_val["pgm"]		= "image/x-portable-graymap";
			this->_val["ppm"]		= "image/x-portable-pixmap";
			this->_val["zip"]		= "multipart/x-zip";
			this->_val["gzip"]		= "multipart/x-gzip";
			this->_val["gz"]		= "multipart/x-gzip";
			this->_val["css"]		= "text/css";
			this->_val["scss"]		= "text/scss";
			this->_val["csv"]		= "text/csv";
			this->_val["htm"]		= "text/html";
			this->_val["html"]		= "text/html";
			this->_val["f90"]		= "text/plain";
			this->_val["m"]			= "text/plain";
			this->_val["hh"]		= "text/plain";
			this->_val["cc"]		= "text/plain";
			this->_val["c"]			= "text/plain";
			this->_val["cpp"]		= "text/plain";
			this->_val["h"]			= "text/plain";
			this->_val["hpp"]		= "text/plain";
			this->_val["g"]			= "text/plain";
			this->_val["txt"]		= "text/plain";
			this->_val["rtx"]		= "text/richtext";
			this->_val["rtf"]		= "text/rtf";
			this->_val["tsv"]		= "text/tab-separated-value";
			this->_val["xml"]		= "text/xml";
			this->_val["h264"]		= "video/h264";
			this->_val["dv"]		= "video/dv";
			this->_val["mpe"]		= "video/mpeg";
			this->_val["mpg"]		= "video/mpeg";
			this->_val["mpeg"]		= "video/mpeg";
			this->_val["qt"]		= "video/quicktime";
			this->_val["mov"]		= "video/quicktime";
			this->_val["avi"]		= "video/msvideo";
		}

		~Mime() { }

		std::string get_mime(std::string ext) {
			MAP_STRING::iterator find = this->_val.find(ext);
			if (find == this->_val.end()) {
				return ("not_found");
			}
			else {
				return (find->second);
			}
		}

};

class MatchCode {
	private:
		MAP_INT_STRING _val;

	public:
	// TODO ADD ALL CODE TO CONFIG
	// sauf code in 200
		MatchCode() {
			this->_val[100] = "Continue";
			this->_val[101] = "Switching Protocols";
			this->_val[102] = "Processing";
			this->_val[103] = "Early Hints";
			
			this->_val[200] = "OK";
			this->_val[201] = "Created";
			this->_val[202] = "Accepted";
			this->_val[203] = "Non-Authoritative Information";
			this->_val[204] = "No Content";
			this->_val[205] = "Reset Content";
			this->_val[206] = "Partial Content";
			this->_val[207] = "Multi-Status";
			this->_val[208] = "Already Reported";
			this->_val[210] = "Content Different";
			this->_val[226] = "IM Used";
			
			this->_val[300] = "Multiple Choices";
			this->_val[301] = "Moved Permanently";
			this->_val[302] = "Found";
			this->_val[303] = "See Other";
			this->_val[304] = "Not Modified";
			this->_val[305] = "Use Proxy (depuis HTTP/1.1)";
			this->_val[306] = "Switch Proxy";
			this->_val[307] = "Temporary Redirect";
			this->_val[308] = "Permanent Redirect";
			this->_val[310] = "Too many Redirects";
			
			this->_val[400] = "Bad Request";
			this->_val[401] = "Unauthorized";
			this->_val[402] = "Payment Required";
			this->_val[403] = "Forbidden";
			this->_val[404] = "Not Found";
			this->_val[405] = "Method Not Allowed";
			this->_val[406] = "Not Acceptable";
			this->_val[407] = "Proxy Authentication Required";
			this->_val[408] = "Request Time-out";
			this->_val[409] = "Conflict";
			this->_val[410] = "Gone";
			this->_val[411] = "Length Required";
			this->_val[412] = "Precondition Failed";
			this->_val[413] = "Request Entity Too Large";
			this->_val[414] = "Request-URI Too Long";
			this->_val[415] = "Unsupported Media Type";
			this->_val[416] = "Requested range unsatisfiable";
			this->_val[417] = "Expectation failed";
			this->_val[418] = "I’m a teapot";
			this->_val[421] = "Bad mapping / Misdirected Request";
			this->_val[422] = "Unprocessable entity";
			this->_val[423] = "Locked";
			this->_val[424] = "Method failure";
			this->_val[425] = "Too Early";
			this->_val[426] = "Upgrade Required";
			this->_val[428] = "Precondition Required";
			this->_val[429] = "Too Many Requests";
			this->_val[431] = "Request Header Fields Too Large";
			this->_val[449] = "Retry With";
			this->_val[450] = "Blocked by Windows Parental Controls";
			this->_val[451] = "Unavailable For Legal Reasons";
			this->_val[456] = "Unrecoverable Error";
			this->_val[444] = "No Response";
			this->_val[495] = "SSL Certificate Error";
			this->_val[496] = "SSL Certificate Required";
			this->_val[497] = "HTTP Request Sent to HTTPS Port";
			this->_val[498] = "Token expired/invalid";
			this->_val[499] = "Client Closed Request";
			
			this->_val[500] = "Internal Server Error";
			this->_val[501] = "Not Implemented";
			this->_val[502] = "Bad Gateway ou Proxy Error";
			this->_val[503] = "Service Unavailable";
			this->_val[504] = "Gateway Time-out";
			this->_val[505] = "HTTP Version not supported";
			this->_val[506] = "Variant Also Negotiates";
			this->_val[507] = "Insufficient storage";
			this->_val[508] = "Loop detected";
			this->_val[509] = "Bandwidth Limit Exceeded";
			this->_val[510] = "Not extended";
			this->_val[511] = "Network authentication required";
			this->_val[520] = "Unknown Error";
			this->_val[521] = "Web Server Is Down";
			this->_val[522] = "Connection Timed Out";
			this->_val[523] = "Origin Is Unreachable";
			this->_val[524] = "A Timeout Occurred";
			this->_val[525] = "SSL Handshake Failed";
			this->_val[526] = "Invalid SSL Certificate";
			this->_val[527] = "Railgun Error";
		}

		~MatchCode() { }

		std::string get_message(int code) {
			return (this->_val[code]);
		}

};

unsigned long					ato_octets			(std::string const &);
std::vector<std::string>		split_NoEscape		(std::string const &, std::string const &);
bool							isTryingtoEscape	(std::string const &);
std::vector<std::string>		ft_split			(std::string const &, std::string const &);
std::pair<std::string, size_t>	check_redirection	(std::string const &, std::string const &);
bool							isValidHTTPversion	(std::string const &);
size_t							endofSpaces			(std::string const &, size_t x);
bool							isValidHost			(std::string const &);
bool							isMethodAllowed		(std::string const & method1, std::string const & method2);
