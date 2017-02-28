/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2017  Mark Samman <mark.samman@gmail.com>
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


#ifndef FS_HTTP_API_COMMON_H_28A59A93B06ABF50122CED74389422A2
#define FS_HTTP_API_COMMON_H_28A59A93B06ABF50122CED74389422A2

#include "common.h"
#include "peer.h"

#include <memory>
#include <functional>

namespace HttpApi
{

class GenericStreamAcceptor
{
public:
	using Pointer = std::unique_ptr<GenericStreamAcceptor>;
	using AcceptHandler = std::function<void(ErrorCode)>;

	static Pointer make(IoService& service, const std::string& address, uint16_t port);

	virtual ~GenericStreamAcceptor() = default;

	virtual void open(const std::string& address, uint16_t port, ErrorCode& error) = 0;
	virtual void close(ErrorCode& error) = 0;
	virtual void asyncAccept(Peer::Socket& socket, AcceptHandler handler) = 0;
};

} //namespace HttpApi

#endif //FS_HTTP_API_COMMON_H_28A59A93B06ABF50122CED74389422A2
