#pragma once

#include <boost/beast/http/status.hpp>
#include <boost/json/value.hpp>

namespace tfs::http {

std::pair<boost::beast::http::status, boost::json::value> handle_serverinfo(const boost::json::object& body,
                                                                            std::string_view ip);

}
