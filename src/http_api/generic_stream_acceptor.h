// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_HTTP_API_GENERIC_STREAM_ACCEPTOR_H
#define FS_HTTP_API_GENERIC_STREAM_ACCEPTOR_H

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
	static Pointer make(asio::io_service& service, const std::string& address, uint16_t port);

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

#endif //FS_HTTP_API_GENERIC_STREAM_ACCEPTOR_H
