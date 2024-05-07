#pragma once

#include "session.h"

#include <boost/beast/http/message_generator.hpp>

namespace tfs::http {

using Request = boost::beast::http::request<boost::beast::http::string_body>;
boost::beast::http::message_generator handle_request(const Request& req, std::string_view ip);

} // namespace tfs::http
