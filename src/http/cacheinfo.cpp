#include "../otpch.h"

#include "cacheinfo.h"

#include "../database.h"
#include "error.h"

std::pair<boost::beast::http::status, boost::json::value> tfs::http::handle_cacheinfo(const boost::json::object&,
                                                                                      std::string_view)
{
	thread_local auto& db = Database::getInstance();
	auto result = db.storeQuery("SELECT COUNT(*) AS `count` FROM `players_online`");
	if (!result) {
		return make_error_response();
	}

	return {boost::beast::http::status::ok, {{"playersonline", result->getNumber<uint32_t>("count")}}};
}
