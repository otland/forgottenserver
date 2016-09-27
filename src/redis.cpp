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

#include "otpch.h"

#include "redis.h"

Redis::Redis(std::string host, int port, boost::asio::io_service &ioService) :
	ioService(ioService),
	publisher(ioService), subscriber(ioService), sync(ioService),
	address(boost::asio::ip::address::from_string(host)), port(port),
	scriptInterface("RedisScripts Interface")
{
	scriptInterface.initState();


	connectSync();
	connectPublisher();
	connectSubscriber();
}

void Redis::stop()
{
	stopped = true;
}

bool Redis::isSuccessfullyInitalized()
{
	return syncConnected && publisherConnected && subscriberConnected;
}

void Redis::connectSync()
{
	std::string errmsg;

	if (sync.connect(address, port, errmsg))
	{
		syncConnected = true;
	}
}

void Redis::connectPublisher()
{
	publisher.connect(address, port, [=](bool status, std::string err) {
		if (status) {
			publisherConnected = true;
		}
	});

	ioService.run_one();
}

void Redis::connectSubscriber()
{
	subscriber.connect(address, port, [=](bool status, std::string err) {
		if (status) {
			subscriberConnected = true;
		}
	});

	ioService.run_one();
}

bool Redis::set(const std::string &key, const std::string &value)
{
	redisclient::RedisValue result = sync.command("SET", { key, value });

	if (result.isError())
	{
		std::cout << "SET error: " << result.toString() << std::endl;
		return false;
	}

	return true;
}

std::string Redis::get(const std::string &key)
{
	redisclient::RedisValue result = sync.command("GET", { key });
	std::string resultString = result.toString();

	if (result.isNull())
		return "0-$NULL";

	if (!result.isOk())
	{
		std::cout << "GET error: " << resultString << std::endl;
		return "0-$NULL";
	}

	return resultString;
}

void Redis::publish(const std::string &channel, const std::string &str)
{
	if (publisherConnected)
	{
		publisher.publish(channel, str);
	}
}

void Redis::subscribe(const std::string &channel)
{
	if (subscriberConnected)
	{
		subscriber.subscribe(channel, [=](const std::vector<char> buf) {
			onMessage(channel, buf);
		});
	}
}

void Redis::onMessage(std::string channel, const std::vector<char> &buf)
{
	if (!stopped)
	{
		std::string msg(buf.begin(), buf.end());
		lua_State* L = scriptInterface.getLuaState();

		lua_getglobal(L, "redisEventCatcher");
		lua_pushstring(L, channel.c_str());
		lua_pushstring(L, msg.c_str());
		lua_pcall(L, 2, 0, 0);
	}
}