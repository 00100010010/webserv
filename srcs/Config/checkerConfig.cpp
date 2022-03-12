#include "SubConfig.hpp"

static int	checkPort(std::string const & value) {
	for (size_t i = 0 ; i < value.size() ; i++) {
		if (isdigit(value[i]))
			continue;
		INFO_SYST("Invalid port : " << value, " is invalid.")
		return 1;
	}
	int port = std::atoi(value.c_str());
	if (port <= 0 || port > 65535) {
		INFO_SYST("Invalid port : " << port, "is out of range (1 - 65535).")
	}
	else
		return 0;
	return 1;
}

static int	checkIP(std::string const & value) {
	std::string 		cpy = value + ".", cpy2;
	std::vector<size_t> numbers;

	if (value == "localhost")
		return 0;
	for (size_t i = 0, y = 0, flag1 = 0, flag2 = 0; i < cpy.size() ; i++) {
		if (cpy[i] == '.' && flag1 < 4 && !flag2) {
			cpy2 = cpy.substr(y, i - y);
			numbers.push_back(std::atoi(cpy2.c_str()));
			y = i + 1;
			flag1++;
			flag2++;
		}
		else if (!std::isdigit(cpy[i])) {
			INFO_SYST("Invalid IP Address : " << value,
					"contains invalid(s) char(s)")
			return 1;
		}
		else if (flag2)
			flag2--;
	}

	if (numbers.size() != 4) {
		INFO_SYST("Invalid IP Address : " << value
					,"contains invalid(s) char(s)")
		return 1;
	}
	for (size_t i = 0; i < 4; i++) {
		if (numbers[i] < 0 || numbers[i] > 255) {
			INFO_SYST("Invalid IP Address : " << value
					,"contains invalid(s) number(s)")
			return 1;
		}
	}
	return 0;
}

int	checker(std::string const & key, std::string const & value)
{
		if (key == "listen")
			return checkPort(value);
		else if (key == "ip")
			return checkIP(value);
		return 0;
}
