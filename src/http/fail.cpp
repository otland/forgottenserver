#include "fail.h"

#include <iostream>

void tfs::http::fail(boost::beast::error_code ec, char const* what)
{
	std::cerr << what << ": " << ec.message() << "\n";
}
