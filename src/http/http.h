#pragma once

#include <chrono>
#include <string_view>

namespace tfs::http {

std::chrono::system_clock::time_point serverStart();

void start(std::string_view address, unsigned short port = 8080, int threads = 1);
void stop();

} // namespace tfs::http
