#####################################################################
GRN			= \033[0;32m
BLU			= \033[0;34m
RED			= \033[0;31m
YEL 		= \033[0;33m
MAG 		= \033[0;35m
CYN 		= \033[0;36m
WHT 		= \033[0;37m
NO_COLOR	= \033[0m
#####################################################################

MULTICOLWBSRV= $(BLU)WB$(CYN)SR$(WHT)VR$(WHT)
NORMALWBSRV	= $(GRN)W E B S E R V E R
CLR_LINE	= \33[2K\r
DEL_BAR_1	= $(BLU)╬$(MULTICOLWBSRV)╬ $(WHT)
DEL_BAR_2	= $(BLU)$(GRN) deleted$(CYN)$(NO_COLOR)
FCLEAN_LINE	= "$(DEL_BAR_1)$(WHT)%s.a$(DEL_BAR_2)\n"	"$(NAME)"
CLEAN_LINE0	= "$(DEL_BAR_1)$(WHT)%-11s$(DEL_BAR_2)\n" 	"$(DIR_OBJ_0)"
CLEAN_LINE1	= "$(DEL_BAR_1)$(WHT)%-11s$(DEL_BAR_2)\n" 	"config.html"
CLEAN_LINE2	= "$(DEL_BAR_1)$(WHT)%-11s$(DEL_BAR_2)\n" 	"config.log"
CLEAN_LINE	= $(DEL_BAR_1)$(WHT)%-11s$(DEL_BAR_2)\n
ALL_LINE00	= "\n$(RED)%12s%34s\n\n%14s$(NORMALWBSRV)%28s\n"
ALL_LINE01	= "$(RED)%37s%28s\n\n$(RED)%12s%34s$(NO_COLOR)\n\n"
ALL_LINE0	= $(ALL_LINE00) "┏━" "━┓" " " " "
ALL_LINE1	= $(ALL_LINE01) "ウェブサーバー" " " "┗━" "━┛"

#####################################################################

NAME 		= webserv
INCLUDE		= -I includes
FSANITIZE	= -g3 -fsanitize=address
CXX			= clang++
CXXFLAGS	= -Werror -Wextra -Wall -std=c++98 $(INCLUDE)  -MMD -MP

#####################################################################

DIR_0		= srcs/
DIR_1		= $(DIR_0)Config/
SRC_LST_0	= $(addsuffix .cpp,	Server \
								main \
								Header \
								Methods \
								ServerLaunch \
								Interact \
								Cleaner \
								Signal \
								utils \
								Cgi)

SRC_LST_1	= $(addsuffix .cpp,	checkerConfig \
								Config \
								ServerConfig \
								SubConfig \
								utilsConfig)

DIR_OBJ_0	= obj/
DIR_OBJ_1	= $(DIR_OBJ_0)Config/
OBJ_LST_0	= $(addprefix $(DIR_OBJ_0), $(SRC_LST_0:.cpp=.o))
OBJ_LST_1	= $(addprefix $(DIR_OBJ_1), $(SRC_LST_1:.cpp=.o))

DEPS_0 = $(OBJ_LST_0:.o=.d)
DEPS_1 = $(OBJ_LST_1:.o=.d)

#####################################################################

all: $(NAME)
		@printf "$(CLR_LINE)"
		@./includes/utils/artGenerator/artGenerator.sh
		@printf $(ALL_LINE0)
		@printf $(ALL_LINE1)

$(NAME): $(OBJ_LST_0) $(OBJ_LST_1)
		@$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ_LST_0) $(OBJ_LST_1) $(INCLUDE)

$(DIR_OBJ_0)%.o: $(DIR_0)%.cpp
		@mkdir -p $(DIR_OBJ_1)
		@printf "${CLR_LINE}"
		@printf "%s" "$@"
		@$(CXX) $(INCLUDE) -o $@ -c $< $(CXXFLAGS)

run: 
		@./$(NAME)

clean:
		@printf "$(CLEAN_LINE)" "$(DIR_OBJ_0)"
		@printf "$(CLEAN_LINE)" "config.log"
		@printf "$(CLEAN_LINE)" "headers.log"
		@rm -rf $(DIR_OBJ_0)
		@rm -rf $(DIR_OBJ_1)
		@rm -rf headers.log
		@rm -rf config.log
		@rm -rf tmp

fclean: clean
		@printf $(FCLEAN_LINE)
		@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re run

-include $(DEPS_0) $(DEPS_1)
