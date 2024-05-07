#define BOOST_ASIO_NO_DEPRECATED

#include "http.h"

#include "listener.h"

#include <fmt/core.h>

#if __has_include("luajit/lua.hpp")
#include <luajit/lua.hpp>
#else
#include <lua.hpp>
#endif

namespace {

boost::asio::io_context ioc;

std::vector<std::thread> workers = {};

} // namespace

void tfs::http::start(std::string_view address, unsigned short port /*= 8080*/, int threads /*= 1*/)
{
	fmt::print(">> Starting HTTP server on {:s}:{:d} with {:d} threads.\n", address, port, threads);

	auto listener = make_listener(ioc, {boost::asio::ip::make_address(address), port});
	listener->run();

	workers.reserve(threads);
	for (auto i = 0; i < threads; ++i) {
		workers.emplace_back([] { ioc.run(); });
	}
}

void tfs::http::stop()
{
	fmt::print(">> Stopping HTTP server...\n");

	ioc.stop();
	for (auto& worker : workers) {
		worker.join();
	}

	fmt::print(">> Stopped HTTP server.\n");
}
