![Build Status](http://jenkins.otland.net/buildStatus/icon?job=forgottenserver-windows)

forgottenserver
===============

The Forgotten Server is a server application written in C++ for the MMORPG Tibia. It is is based on the OpenTibia Server project.

#### Building

On most systems you can build the server using the following commands:

	$ mkdir build && cd build
	$ cmake ..
	$ make

To build the Release variant, use
    
    $ cmake -DCMAKE_BUILD_TYPE=Release ..
