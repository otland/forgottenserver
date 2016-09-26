/**
* The Forgotten Server - a free and open-source MMORPG server emulator
* Copyright (C) 2016  Mark Samman <mark.samman@gmail.com>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef FS_GAME_H_415c7e06e9eafb77540b29f60a9ab327
#define FS_GAME_H_415c7e06e9eafb77540b29f60a9ab327

#include <string>
#include <iostream>
#include <functional>

#include <redisclient/redissyncclient.h>
#include <redisclient/redisasyncclient.h>

#include <boost/format.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/address.hpp>

#include "luascript.h"

class Redis {
public:
	Redis(std::string host, int port, boost::asio::io_service &ioService);

	void stop();
	bool isSuccessfullyInitalized();

	void publish(const std::string &channel, const std::string &str);
	void subscribe(const std::string &channel);

	bool set(const std::string &key, const std::string &value);
	std::string get(const std::string &key);

private:
	const boost::asio::ip::address address;
	const unsigned short port;

	bool stopped = false;

	bool syncConnected = false;
	bool publisherConnected = false;
	bool subscriberConnected = false;

	LuaScriptInterface scriptInterface;

	boost::asio::io_service &ioService;

	redisclient::RedisSyncClient sync;
	redisclient::RedisAsyncClient publisher;
	redisclient::RedisAsyncClient subscriber;

	void connectSync();
	void connectPublisher();
	void connectSubscriber();

	void onMessage(std::string channel, const std::vector<char> &buf);
};
#endif