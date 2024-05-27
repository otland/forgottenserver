#pragma once

#include <string_view>

namespace tfs::http {

void start(std::string_view address, unsigned short port = 8080, int threads = 1);
void stop();

} // namespace tfs::http
