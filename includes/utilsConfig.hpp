#ifndef UTILSCONFIG_HPP
# define UTILSCONFIG_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include "utils/colors.hpp"
#include "utils/msg_sys.hpp"

void		truncSpaces			(std::string &);
void		eraseSpaces			(std::string &);
int			isCommentLine		(std::string const &);
int			isEndPart			(std::string const &);
int			openFile			(std::ifstream &, const char *);
int			isNextServ			(std::string const &);
int			isLastPartClosed	(size_t, std::string const&,std::string const &);
int			isNextLocation		(std::string const & line);
std::string	extractLocationName	(std::string const & line);
void		toLower				(std::string & line);
void		toUpper				(std::string & line);
std::vector<std::string> ft_split2(std::string const &, std::string const &);

#endif
