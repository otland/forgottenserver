#pragma once

#include <boost/beast/http/status.hpp>
#include <boost/json/value.hpp>

namespace beast = boost::beast;
namespace json = boost::json;

namespace tfs::http {

namespace detail {

struct ErrorResponseParams
{
	int code = 2;
	std::string_view message =
	    "Internal error. Please try again later or contact customer support if the problem persists.";
	beast::http::status status = beast::http::status::bad_request;
};

} // namespace detail

std::pair<beast::http::status, json::value> make_error_response(detail::ErrorResponseParams params = {});

} // namespace tfs::http
