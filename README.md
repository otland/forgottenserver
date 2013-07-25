forgottenserver
===============

The Forgotten Server is a server application written in C++ for the MMORPG Tibia. It is is based on the OpenTibia Server project.

### Installing required libraries

##### Debian GNU/Linux, Ubuntu

	# apt-get install build-essential libboost-dev libxml2-dev liblua5.2-dev libgmp3-dev \
	  libmysqlclient-dev libboost-thread-dev libboost-regex-dev libboost-system-dev cmake

##### Mac OS X

	$ ruby -e "$(curl -fsSL https://raw.github.com/mxcl/homebrew/go)"
	$ brew install boost gmp libxml2 mysql-connector-c lua52 cmake

##### Other systems

You will need the following libraries:
* [Boost](http://boost.org/)
* [Libxml2](http://www.xmlsoft.org/)
* [GMP](http://gmplib.org/)
* [Lua](http://www.lua.org/)
* [MySQL C connector](http://dev.mysql.com/downloads/connector/c/)

If you want to use the building instructions below, you will also need to install [CMake](http://www.cmake.org/) to generate the build files for your compiler.

### Building

On most systems you can build the server using the following commands:

	$ mkdir build && cd build
	$ cmake ..
	$ make

To build the Release variant, use
    
    $ cmake -DCMAKE_BUILD_TYPE=Release ..
