#define BOOST_TEST_MODULE base64

#include "../otpch.h"

#include "../base64.h"

#include <boost/test/unit_test.hpp>

struct Basee64Fixture
{
	std::string_view plain;
	std::string_view encoded;
};

// test vectors from https://datatracker.ietf.org/doc/html/rfc4648#section-10
auto testVectors = std::vector<Basee64Fixture>{
    {.plain = "", .encoded = ""},
    {.plain = "f", .encoded = "Zg=="},
    {.plain = "fo", .encoded = "Zm8="},
    {.plain = "foo", .encoded = "Zm9v"},
    {.plain = "foob", .encoded = "Zm9vYg=="},
    {.plain = "fooba", .encoded = "Zm9vYmE="},
    {.plain = "foobar", .encoded = "Zm9vYmFy"},

};

BOOST_AUTO_TEST_CASE(test_base64_encode)
{
	for (auto&& [plain, encoded] : testVectors) {
		std::string result = tfs::base64::encode(plain);
		BOOST_TEST(result == encoded, "expected '" << encoded << "', got '" << result << "'");
	}
}

BOOST_AUTO_TEST_CASE(test_base64_decode)
{
	for (auto&& [plain, encoded] : testVectors) {
		std::string result = tfs::base64::decode(encoded);
		BOOST_TEST(result == plain, "expected '" << plain << "', got '" << result << "'");
	}
}
