#!/bin/bash

RED='\e[31m'
GREEN='\e[32m'
YELLOW='\e[33m'
NC='\033[0m' # No Color

##########################
###Check for runnig database containers
##########################
DB_CONTAINER=$(docker ps -a | grep tfs-mysql)
if [ "$DB_CONTAINER" == '' ];
then
	echo -e "${RED} [ERROR]:  tfs-mysql container before performing a database dump ${NC}";
	exit 1;
fi;

#########################
###Parse settings. ini file from /home
#########################
sudo touch  /home/tfs-settings.ini
source /home/tfs-settings.ini

#########################
###Ensure dump directory exists
#########################
echo  -e  "${GREEN}[NOTICE]: Ensure dump directory exists ${NC}";
sudo mkdir -p $sql_dumps_path


##########################
###Check for runnig database containers
##########################
DB_CONTAINER=$(docker ps | grep tfs-mysql)
if [ "$DB_CONTAINER" == '' ];
then
	echo -e "${RED} [ERROR]: Run tfs-mysql container before performing a database dump ${NC}";
	exit 1;
fi;

##########################################
###Drop existing database on local machine
##########################################
echo  -e  "${GREEN}[NOTICE]: Drop existing database on local machine ${NC}"
docker exec tfs-mysql mysql -u$sql_user -p$sql_password -e "DROP DATABASE IF  EXISTS ${sql_database}; CREATE DATABASE ${sql_database}; USE ${sql_database}";

echo  -e  "${GREEN}[NOTICE]: Installing schema database.${NC}"
docker exec -i tfs-mysql mysql -u$sql_user -p$sql_password  ${sql_database} < schema.sql;

echo  -e  "${GREEN}[NOTICE]: Synchronization done.${NC}"
