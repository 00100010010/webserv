#ifndef MSG_SYS
# define MSG_SYS

#include <iostream>
#include "utils/colors.hpp"

#define DBG_FLAG 0
#define OUTPUT_FLAG 0

#define RST_ENDL		RESETCOLOR << std::endl
#define GRNFLASH		GRN << "\t>> " << RESETCOLOR
#define GRNCROCHETOS(x) GRN << "[" << x << GRN "]" << RESETCOLOR

#define INFO_LOGO	GRNCROCHETOS(RED << "/" << YEL << "!" << RED << "\\")
#define ERR_LOGO	GRNCROCHETOS(RED << "ERR")

#define DBG(x) if (DBG_FLAG) { std::cout << RED << "[DBG]\t" << x << RST_ENDL; }
#define ERR_SYST(x,y) if (OUTPUT_FLAG) { std::cerr << ERR_LOGO << GRNFLASH << GRN << x << ":\t\t\t" << RED << y << RST_ENDL ; }
#define MSG_SYST(x)  if (OUTPUT_FLAG) { std::cout << GRNFLASH << GRN << x << RST_ENDL; }
#define INFO_SYST(x,y) if (OUTPUT_FLAG) { std::cout << INFO_LOGO << GRNFLASH << GRN << x << ":\t\t" << YEL << y << RST_ENDL; }

#define GOODBYE_MSG std::cout << GRN << "\n\n\n\n\n\tGood bye...\n      👻\n\n\n" << RESETCOLOR;

#endif
