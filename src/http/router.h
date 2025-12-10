#pragma once

#include <boost/beast/http/message_generator.hpp>
#include <boost/beast/http/string_body.hpp>

namespace beast = boost::beast;
namespace json = boost::json;

namespace tfs::http {

beast::http::message_generator handle_request(const beast::http::request<beast::http::string_body>& req,
                                              std::string_view ip);

} // namespace tfs::http
