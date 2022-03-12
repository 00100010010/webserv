#include "utilsConfig.hpp"

void truncSpaces(std::string & line) {
	size_t i = line.find_first_not_of(" \t\n\r\v\f");
	line.erase(0, i);
	i = line.find_last_not_of(" \t\n\r\v\f");
	line.erase(i + 1, line.size());
}

void eraseSpaces(std::string & line) {
	truncSpaces(line);
	if (line.empty())
		return ;

	size_t start;
	for (size_t i = line.find_first_of(" \t\n\r\v\f");
		i != std::string::npos; i = line.find_first_of(" \t\n\r\v\f")) {
		start = i;
		while (std::isspace(line[i]))
			i++;
		line.erase(start, i - start);
	}
}

void	toLower(std::string & line) {
	for (size_t i = 0; i < line.size() ; i++)
		line[i] = std::tolower(line[i]);
}

void	toUpper(std::string & line) {
	for (size_t i = 0; i < line.size() ; i++)
		line[i] = std::toupper(line[i]);
}

int	isEndPart(std::string const & line) {
	std::string cpy = line;
	truncSpaces(cpy);
	if (cpy == "}")
		return 1;
	return 0;
}

int	isNextLocation(std::string const & line) {
	std::string cpy = line, part1 = cpy;
	eraseSpaces(part1);

	if (part1.size() <= 8 || part1[part1.size() - 1] != '{')
		return 0;

	part1 = part1.substr(0, 8);
	if (part1 != "location" || cpy.find_first_of(" \t\n\r\v\f") != 8)

		return 0;
	return 1;
}

int isLastPartClosed(size_t flag, std::string const & filename,
	std::string const & type) {
	if (flag % 2 != 0) {
		INFO_SYST("Config file \"" << filename << "\\", "doesnt close "
				<< type << " part with \'}\'.")
		return 0;
	}
	return 1;
}

int	openFile(std::ifstream & file, const char * filename) {
	file.open(filename);
	if (!file.is_open()) {
		INFO_SYST("Config file \"" << filename << "\\", "couldn't be opened.")
		return -1;
	}
	return 0;
}

std::string extractLocationName(std::string const & line) {
	std::string cpy = line;
	eraseSpaces(cpy);
	std::string name = cpy.substr(8, cpy.size() - 9);
	return name;
}

int	isNextServ(std::string const & line) {
	std::string cpy = line;
	eraseSpaces(cpy);
	if (cpy == "server{")
		return 1;
	return 0;
}

int	isCommentLine(std::string const & line) {
	std::string cpy = line;
	truncSpaces(cpy);
	if (cpy.find('#') != std::string::npos) {
		if (cpy[0] == '#')
			return 1;
	}
	return 0;
}

std::vector<std::string> ft_split2(std::string const & line_origin,
									std::string const & delimiter) {
	std::vector<std::string>	vct;
	std::string					line	= line_origin;
	size_t						x		= 0;
	while (((x = line.find(delimiter, x))) != std::string::npos) {
		if (x && line[x - 1] == '\\') {
			x++;
			continue;
		}
		vct.push_back(line.substr(0, x));
		line.erase(0, x + 1);
		x = 0;
	}
	if (!line.empty())
		vct.push_back(line);
	return vct;
}