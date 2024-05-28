#include "../otpch.h"

#include "cacheinfo.h"

#include "../database.h"
#include "error.h"

namespace beast = boost::beast;
namespace json = boost::json;
using boost::beast::http::status;

std::pair<status, json::value> tfs::http::handle_cacheinfo(const json::object&, std::string_view)
{
	thread_local auto& db = Database::getInstance();
	auto result = db.storeQuery("SELECT COUNT(*) AS `count` FROM `players_online`");
	if (!result) {
		return make_error_response();
	}

	return {status::ok, {{"playersonline", result->getNumber<uint32_t>("count")}}};
}
