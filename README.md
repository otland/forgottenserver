forgottenserver [![Build Status](https://travis-ci.org/otland/forgottenserver.svg?branch=master)](https://travis-ci.org/otland/forgottenserver "Travis CI status") [![Build status](https://ci.appveyor.com/api/projects/status/599x38f3a0luessl?svg=true)](https://ci.appveyor.com/project/otland/forgottenserver "Download builds for Windows") [![Docker status](https://images.microbadger.com/badges/image/otland/forgottenserver.svg)](https://microbadger.com/images/otland/forgottenserver "Docker image status")
===============

The Forgotten Server is a free and open-source MMORPG server emulator written in C++. It is a fork of the [OpenTibia Server](https://github.com/opentibia/server) project. To connect to the server, you can use [OTClient](https://github.com/edubart/otclient) or [OpenTibiaUnity](https://github.com/slavidodo/OpenTibia-Unity).

### Getting Started

* [Compiling](https://github.com/otland/forgottenserver/wiki/Compiling), alternatively download [AppVeyor builds for Windows](https://ci.appveyor.com/project/otland/forgottenserver)
* [Scripting Reference](https://github.com/otland/forgottenserver/wiki/Script-Interface)

### Support

If you need help, please visit the [support forum on OTLand](https://otland.net/forums/support.16/). Our issue tracker is not a support forum, and using it as one will result in your issue being closed. If you were unable to get assistance in the support forum, you should consider [becoming a premium user on OTLand](https://otland.net/account/upgrades) which grants you access to the premium support forum and supports OTLand financially.

### Issues

We use the [issue tracker on GitHub](https://github.com/otland/forgottenserver/issues). Keep in mind that everyone who is watching the repository gets notified by e-mail when there is activity, so be thoughtful and avoid writing comments that aren't meaningful for an issue (e.g. "+1"). If you'd like for an issue to be fixed faster, you should either fix it yourself and submit a pull request, or place a bounty on the issue.

## Docker Compose

There is also a way of running the server + database at once using `docker-compose`.
By default it:
 * Creates a database named `forgottenserver`.
 * Executes `schema.sql` file, so that you don't have to manually import it.
 * Saves all the database files inside a `./db` local directory.
 
##### Preparation:

Update your `configu.lua.dist` with these values, so that server properly connects to the database:

```lua
-- MySQL
mysqlHost = "db"
mysqlUser = "forgottenserver"
mysqlPass = "<your_db_password>"
mysqlDatabase = "forgottenserver"
mysqlPort = 3306
mysqlSock = ""
```

##### Start:

```bash
docker-compose up -d
```

##### Stop:
```bash
docker-compose down
```

##### Rebuild container after source code changes:

```bash
docker-compose up -d --build
```

#### Created services:

1. The Forgotten Server - `localhost:7171`, `localhost:7172`.
2. Database - direct connection hidden to the outside world.
3. Database explorer - `localhost:8080`. It allows viewing what's inside the database.

#### Docker-compose issues:

1. During startup the server container is created after database, however, the database might not be initially available.
This is why the server will restart a couple of times before it successfully establishes the connection.

2. If you are using Docker Toolbox for Windows and it uses VirtualBox,
 then the server and database explorer address host name will be `192.168.99.100` insead of `localhost`.