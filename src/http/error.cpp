#include "error.h"

std::pair<boost::beast::http::status, boost::json::value> tfs::http::make_error_response(
    detail::ErrorResponseParams params /*= {}*/)
{
	boost::json::object body;
	body["errorCode"] = params.code;
	body["errorMessage"] = params.message;

	return std::make_pair(boost::beast::http::status::ok, body);
}
