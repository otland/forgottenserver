## Local docker dev

If you have Docker installed, you can run a server in a container using:
```
docker compose up
```
(first copy `config.lua.dist` into `config.lua`)

It uses ports 7171, 7172, 8080, 8081 and 3307. These ports must be not used by another application. 

Tibia Client should log in to URL `http://127.0.0.1:8080`.

Account e-mail is `tfs@tfs`and password is `tfs`. There are characters `GOD` and one for each vocation on account.

OTS files are in real-time synchronization. You can do changes in `data`/`config.lua` and run `/reload all` on GOD character to load changes. You don't have to restart OTS.

Database data is not stored outside docker container. Every `docker compose down`, it resets a database to state from `docker/data.sql`. If you want to add more accounts/characters, put them in that `.sql` file.

#### Database management

MariaDB database is running on port `3307`. You can connect to it using any database management tool with config:
- host: `127.0.0.1`
- port: `3307`
- username: `root`
- password: `root`
- database name: `forgottenserver`

There is also phpMyAdmin running in a container available on port `8081`. You can visit it: http://127.0.0.1:8081/index.php?route=/database/structure&db=forgottenserver

#### Docker management

To start server:
```
docker compose up
```

To shut down server, press `CTRL+C`.

To reset database:
```
docker compose down
```

To compile TFS again:
```
docker compose build
```
If you often compile and compilation is slow, you can try to replace `RelWithDebInfo` with `Debug` in 2 places in `Dockerfile`.
