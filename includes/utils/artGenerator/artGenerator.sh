#!/usr/bin/env bash
################################################################################################################################################################
RED="\e[0;31m"
GRN="\e[0;32m"
YEL="\e[0;33m"
BLU="\e[0;34m"
MAG="\e[0;35m"
CYN="\e[0;36m"
WHT="\e[0;37m"
HRED="\e[0;91m"
HGRN="\e[0;92m"
HYEL="\e[0;93m"
HBLU="\e[0;94m"
HMAG="\e[0;95m"
HCYN="\e[0;96m"
RESET="\e[0;0m"
################################################################################################################################################################
RAND=$((0 + $RANDOM % 14))
################################################################################################################################################################
FOLDER="includes/utils/artGenerator/srcs/"
FOLDER_TITLE="includes/utils/artGenerator/srcs/title/title"
################################################################################################################################################################

printf "${GRN}"

cat ${FOLDER}${RAND}.art

# printf "${HGRN}"
# cat ${FOLDER_TITLE}1.art

# printf "${GRN}"
# cat ${FOLDER_TITLE}2.art

# printf "${CYN}"
# cat ${FOLDER_TITLE}3.art

# printf "${HBLU}"
# cat ${FOLDER_TITLE}4.art

# printf "${BLU}"
# cat ${FOLDER_TITLE}5.art

# printf "${GRN}"
# cat ${FOLDER_TITLE}6.art

printf "${RESET}"
