#define BOOST_ASIO_NO_DEPRECATED

#include "http.h"

#include "listener.h"

#include <print>
#include <thread>

namespace asio = boost::asio;

namespace {

asio::io_context ioc;

std::vector<std::thread> workers = {};

} // namespace

void tfs::http::start(std::string_view address, unsigned short port /*= 8080*/, int threads /*= 1*/)
{
	if (port == 0 || threads < 1) {
		return;
	}

	std::println(">> Starting HTTP server on {:s}:{:d} with {:d} threads.", address, port, threads);

	auto listener = make_listener(ioc, {asio::ip::make_address(address), port});
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

	std::println(">> Stopping HTTP server...");

	ioc.stop();
	for (auto& worker : workers) {
		worker.join();
	}

	std::println(">> Stopped HTTP server.");
}
