#pragma once

#include <boost/beast/core/error.hpp>

namespace tfs::http {

void fail(boost::beast::error_code ec, char const* what);

}
