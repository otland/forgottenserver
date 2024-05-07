#include "router.h"

#include "cacheinfo.h"
#include "error.h"
#include "login.h"

#include <boost/json/monotonic_resource.hpp>
#include <boost/json/parse.hpp>
#include <boost/json/serialize.hpp>

namespace {

std::pair<boost::beast::http::status, boost::json::value> router(std::string_view type, const boost::json::object& body,
                                                                 std::string_view ip)
{
	using namespace tfs::http;

	if (type == "cacheinfo") {
		return handle_cacheinfo(body, ip);
	}
	if (type == "login") {
		return handle_login(body, ip);
	}

	return make_error_response();
}

thread_local boost::json::monotonic_resource mr;

} // namespace

boost::beast::http::message_generator tfs::http::handle_request(const Request& req, std::string_view ip)
{
	auto&& [status, responseBody] = [&req, ip]() {
		boost::json::error_code ec;
		auto requestBody = boost::json::parse(req.body(), ec, &mr);
		if (ec || !requestBody.is_object()) {
			return make_error_response({.code = 2, .message = "Invalid request body."});
		}

		const auto& requestBodyObj = requestBody.get_object();
		auto typeField = requestBodyObj.if_contains("type");
		if (!typeField || !typeField->is_string()) {
			return make_error_response({.code = 2, .message = "Invalid request type."});
		}

		return router(typeField->get_string(), requestBodyObj, ip);
	}();

	boost::beast::http::response<boost::beast::http::string_body> res{status, req.version()};
	res.body() = boost::json::serialize(responseBody);
	res.keep_alive(req.keep_alive());
	res.prepare_payload();
	return res;
}
