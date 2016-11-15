// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "peer.h"
#include "server.h"
#include "router.h"
#include "responder.h"
#include "../tasks.h"

namespace HttpApi
{

using Minutes = boost::posix_time::minutes;

/** \brief Determines the time after which a connection is closed due to inactivity
 */
const static Minutes TIMER_TIMEOUT{30};

Peer::Peer(Server& server, Router& router, PeerId peerId) :
	server(server),
	router(router),
	socket(server.getIoService()),
	timer(server.getIoService()),
	strand(server.getIoService()),
	peerId(peerId)
{
	//
}

void Peer::onAccept()
{
	auto sharedThis = shared_from_this();
	g_dispatcher.addTask(createTask([sharedThis, this]() {
		router.handleSessionOpen(peerId);
	}));
	read();
}

void Peer::read()
{
	auto sharedThis = shared_from_this();
	startTimer();
	boost::beast::http::async_read(socket, buffer, request, boost::asio::bind_executor(strand, [sharedThis, this](ErrorCode err, const long unsigned int& _) {
		if (err) {
			std::cerr << "HTTP API peer read() error: " << err.message() << std::endl;
			internalClose();
			return;
		}

		cancelTimer(); // cancel read timer
		requestKeepAlive = request.keep_alive();

		startTimer(); // start response generation timer
		g_dispatcher.addTask(createTask([sharedThis, this]() {
			router.handleRequest(Responder{sharedThis, std::move(request), requestCounter}, peerId);
		}));
	}));
}


void Peer::write()
{
	cancelTimer(); // cancel response generation timer
	response.prepare_payload();
	auto sharedThis = shared_from_this();
	startTimer(); // start write timer
	++requestCounter;
	boost::beast::http::async_write(socket, response, boost::asio::bind_executor(strand, [sharedThis, this](ErrorCode err, const long unsigned int& _) {
		if (err) {
			std::cerr << "HTTP API peer write() error: " << err.message() << std::endl;
			internalClose();
			return;
		}

		if (requestKeepAlive && response.keep_alive()) {
			cancelTimer(); // cancel write timer
			read();
		} else {
			internalClose();
		}
	}));
}

void Peer::internalClose()
{
	ErrorCode err{};
	socket.shutdown(asio::socket_base::shutdown_both, err);
	socket.close(err);
	cancelTimer();
	auto sharedThis = shared_from_this();
	g_dispatcher.addTask(createTask([sharedThis, this]() {
		router.handleSessionClose(peerId);
	}));
	server.onPeerClose(*this);
}

void Peer::close()
{
	auto sharedThis = shared_from_this();
	strand.dispatch([sharedThis]() {
		sharedThis->internalClose();
	});
}

void Peer::startTimer()
{
	timer.expires_from_now(TIMER_TIMEOUT);
	PeerWeakPtr weakThis = shared_from_this();
	timer.async_wait(strand.wrap([weakThis](ErrorCode err) {
		if (err == asio::error::operation_aborted) {
			return;
		}

		auto sharedThis = weakThis.lock();
		if (sharedThis == nullptr) {
			return;
		}

		sharedThis->internalClose();
	}));
}

void Peer::cancelTimer()
{
	ErrorCode err{};
	timer.cancel(err);
	if (err) {
		std::cerr << "HTTP API Peer timer cancel error: " << err.message() << std::endl;
	}
}

void Peer::send(Response response, RequestID requestID)
{
	auto sharedThis = shared_from_this();
	strand.dispatch([sharedThis, this, requestID, response]() {
		if (requestCounter != requestID) {
			std::cerr << "HTTP API Peer send error: invalid request ID" << std::endl;
			return;
		}
		this->response = response;
		write();
	});
}

} //namespace HttpApi
