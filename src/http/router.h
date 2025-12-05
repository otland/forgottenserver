#pragma once

#include <boost/beast/http/message_generator.hpp>
#include <boost/beast/http/string_body.hpp>

namespace tfs::http {

boost::beast::http::message_generator handle_request(
    const boost::beast::http::request<boost::beast::http::string_body>& req, std::string_view ip);

} // namespace tfs::http
