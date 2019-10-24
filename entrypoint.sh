#!/bin/sh
set -e 
set -x

TFS_MYSQL_HOST="${TFS_MYSQL_HOST:-127.0.0.1}"
TFS_MYSQL_USER="${TFS_MYSQL_USER:-forgottenserver}"
TFS_MYSQL_PASS="${TFS_MYSQL_PASS:-forgottenserver}"
TFS_MYSQL_DATABASE="${TFS_MYSQL_DATABASE:-forgottenserver}"
TFS_MYSQL_PORT="${TFS_MYSQL_PORT:-3306}"
TFS_MYSQL_SOCK="${TFS_MYSQL_SOCK}"

if [ -f "config.lua.dist" ]; then
    sed -i "s/mysqlHost =.*/mysqlHost = \"${TFS_MYSQL_HOST}\"/" config.lua.dist
    sed -i "s/mysqlUser =.*/mysqlUser = \"${TFS_MYSQL_USER}\"/" config.lua.dist
    sed -i "s/mysqlPass =.*/mysqlPass = \"${TFS_MYSQL_PASS}\"/" config.lua.dist
    sed -i "s/mysqlDatabase =.*/mysqlDatabase = \"${TFS_MYSQL_DATABASE}\"/" config.lua.dist
    sed -i "s/mysqlPort =.*/mysqlPort = ${TFS_MYSQL_PORT}/" config.lua.dist
    sed -i "s/mysqlSock =.*/mysqlSock = \"${TFS_MYSQL_Sock}\"/" config.lua.dist

    mv config.lua.dist config.lua
fi

if [ "${@}" != "" ]; then
    ${@}
    exit $?
fi

if [ -f schema.sql ]; then
    ln -s  /usr /usr/usr
    mysql -h "${TFS_MYSQL_HOST}" -u "${TFS_MYSQL_USER}" "${TFS_MYSQL_DATABASE}" "-p${TFS_MYSQL_PASS}" < schema.sql || true
    mv schema.sql schema.sql.bak
fi

/bin/tfs
