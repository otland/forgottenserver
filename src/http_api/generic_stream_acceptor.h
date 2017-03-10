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
/** \brief Represents an acceptor socket that uses either a local UNIX socket or a normal TCP/IP
 * acceptor.
 *
 * Selection of socket type is done based on the address: if unix:// prefix is present then
 * the socket type will be a local socket, assuming the underlying OS supports them.
 * \remark Thread-unsafe - access to objects of this class should be sychronized externally.
 */
class GenericStreamAcceptor : NonCopyable, NonMovable
{
public:
	using Pointer = std::unique_ptr<GenericStreamAcceptor>;
	using AcceptHandler = std::function<void(ErrorCode)>;

	/** \brief Virtual constructor
	 *
	 * Constructs a \ref GenericStreamAcceptor of the appropriate type based on the presence of the
	 * address prefix.
	 *
	 * \param[in] service the asio io_service object.
	 * \param[in] address the address on which the acceptor will listen for connections.
	 * \param[in] port the port on which the acceptor will listen for incoming connections.
	 *
	 * \returns a unique_ptr pointing to the constructed object if successful (address is valid),
	 * otherwise the pointer is nullptr.
	 */
	static Pointer make(IoService& service, const std::string& address, uint16_t port);

	virtual ~GenericStreamAcceptor() = default;

	/** \brief Opens the acceptor for incoming connections
	 *
	 * Opens the acceptor, binds it to \ref address and starts asynchronously listening for connections.
	 *
	 * \param[in] address the address on which the acceptor will listen for connections.
	 * \param[in] port the port on which the acceptor will listen for incoming connections.
	 *
	 * \param[out] error the resulting error code. On success error evaluates to false.
	 */
	virtual void open(const std::string& address, uint16_t port, ErrorCode& error) = 0;

	/** \brief Closes the acceptor
	 * The acceptor is closed and will no longer accept incoming connections. Cancels all
	 * pending asynchronous operations on this socket.
	 *
	 * \param[out] error the resulting error code. On success error evaluates to false.
	 */
	virtual void close(ErrorCode& error) = 0;

	/** \brief Enqueues an asynchronous accept operation
	 *
	 * Starts asynchronously listening for incoming connections. Returns immediately and any errors are
	 * propagated to the handler.
	 * \remark Responsibility for managing the lifetime of \ref socket lies on the caller.
	 *
	 * \param[out] socket the socket that will represent the connection, if operation completes successfully
	 * \param[in] handler the handler callback that is executed when the operation is completed.
	 */

	virtual void asyncAccept(Peer::Socket& socket, AcceptHandler handler) = 0;
};

} //namespace HttpApi

#endif //FS_HTTP_API_COMMON_H_28A59A93B06ABF50122CED74389422A2
