#define BOOST_TEST_MODULE rsa

#include "../otpch.h"

#include "../tools.h"

#include <boost/test/unit_test.hpp>

using namespace std::string_view_literals;

auto testVectors = std::array{
    std::pair{""sv, "\xda\x39\xa3\xee\x5e\x6b\x4b\x0d\x32\x55\xbf\xef\x95\x60\x18\x90\xaf\xd8\x07\x09"sv},
    std::pair{"The quick brown fox jumps over the lazy dog"sv,
              "\x2f\xd4\xe1\xc6\x7a\x2d\x28\xfc\xed\x84\x9e\xe1\xbb\x76\xe7\x39\x1b\x93\xeb\x12"sv},
};

BOOST_AUTO_TEST_CASE(test_sha1)
{
	for (auto&& [input, expected] : testVectors) {
		std::string result = transformToSHA1(input);
		BOOST_TEST(result == expected, "expected '" << expected << "', got '" << result << "'");
	}
}
