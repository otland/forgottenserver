#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <string_view>
#include <thread>

namespace tfs::http {

void start(std::string_view address, unsigned short port = 8080, int threads = 1);
void stop();

} // namespace tfs::http
