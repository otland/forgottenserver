#define BOOST_ASIO_NO_DEPRECATED

#include "http.h"

#include "listener.h"

#include <fmt/core.h>
#include <thread>

namespace asio = boost::asio;

namespace {

asio::io_context ioc;

std::vector<std::thread> workers = {};

} // namespace

void tfs::http::start(bool bindOnlyOtsIP, std::string_view otsIP, unsigned short port /*= 8080*/, int threads /*= 1*/)
{
	if (port == 0 || threads < 1) {
		return;
	}

	asio::ip::address address = asio::ip::address_v6::any();
	if (bindOnlyOtsIP) {
		address = asio::ip::make_address(otsIP);
	}
	fmt::print(">> Starting HTTP server on {:s}:{:d} with {:d} threads.\n", address.to_string(), port, threads);

	auto listener = make_listener(ioc, {address, port});
	listener->run();

	workers.reserve(threads);
	for (auto i = 0; i < threads; ++i) {
		workers.emplace_back([] { ioc.run(); });
	}
}

void tfs::http::stop()
{
	if (workers.empty()) {
		return;
	}

	fmt::print(">> Stopping HTTP server...\n");

	ioc.stop();
	for (auto& worker : workers) {
		worker.join();
	}

	fmt::print(">> Stopped HTTP server.\n");
}
