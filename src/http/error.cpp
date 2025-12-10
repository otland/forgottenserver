#include "error.h"

std::pair<beast::http::status, json::value> tfs::http::make_error_response(detail::ErrorResponseParams params /*= {}*/)
{
	json::object body;
	body["errorCode"] = params.code;
	body["errorMessage"] = params.message;

	return std::make_pair(beast::http::status::ok, body);
}
