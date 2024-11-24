#include "error.h"

namespace beast = boost::beast;
namespace json = boost::json;
using boost::beast::http::status;

std::pair<status, json::value> tfs::http::make_error_response(detail::ErrorResponseParams params /*= {}*/)
{
	json::object body;
	body["errorCode"] = params.code;
	body["errorMessage"] = params.message;

	return std::make_pair(status::ok, body);
}
