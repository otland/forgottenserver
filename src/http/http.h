#pragma once

#include <chrono>
#include <string_view>

namespace tfs::http {

std::chrono::system_clock::duration uptime();

void start(bool bind_to_specific_ip, std::string_view ip, unsigned short port = 8080, int threads = 1);
void stop();

} // namespace tfs::http
