#pragma once

#include <boost/beast/http/status.hpp>
#include <boost/json/value.hpp>

namespace tfs::http {

// `proxied` tells whether the client connected through a local proxy (PROXY protocol), in which case it is told to
// keep going through it to reach the game server
std::pair<boost::beast::http::status, boost::json::value> handle_login(const boost::json::object& body,
                                                                       std::string_view ip, bool proxied = false);

} // namespace tfs::http
