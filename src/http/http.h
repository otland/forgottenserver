#pragma once

#include <string_view>

namespace tfs::http {

void start(bool onlyGlobalIP, std::string_view otsIP, unsigned short port = 8080, int threads = 1);
void stop();

} // namespace tfs::http
