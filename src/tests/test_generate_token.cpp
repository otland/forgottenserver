#define BOOST_TEST_MODULE rsa

#include "../otpch.h"

#include "../tools.h"

#include <boost/test/unit_test.hpp>

struct HmacSHA1Fixture
{
	std::string key;
	std::string message;
	std::string_view expected;
};

BOOST_AUTO_TEST_CASE(test_hmac_sha1)
{
	using namespace std::string_view_literals;

	// test vectors from https://www.rfc-editor.org/rfc/rfc2202.html#section-2
	auto testVectors = std::vector<HmacSHA1Fixture>{
	    {.key = std::string(20, '\x0b'),
	     .message = "Hi There",
	     .expected = "\xb6\x17\x31\x86\x55\x05\x72\x64\xe2\x8b\xc0\xb6\xfb\x37\x8c\x8e\xf1\x46\xbe\x00"sv},
	    {.key = "Jefe",
	     .message = "what do ya want for nothing?",
	     .expected = "\xef\xfc\xdf\x6a\xe5\xeb\x2f\xa2\xd2\x74\x16\xd5\xf1\x84\xdf\x9c\x25\x9a\x7c\x79"sv},
	    {.key = std::string(20, '\xaa'),
	     .message = std::string(50, '\xdd'),
	     .expected = "\x12\x5d\x73\x42\xb9\xac\x11\xcd\x91\xa3\x9a\xf4\x8a\xa1\x7b\x4f\x63\xf1\x75\xd3"sv},
	    {.key = "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19",
	     .message = std::string(50, '\xcd'),
	     .expected = "\x4c\x90\x07\xf4\x02\x62\x50\xc6\xbc\x84\x14\xf9\xbf\x50\xc8\x6c\x2d\x72\x35\xda"sv},
	    {.key = std::string(20, '\x0c'),
	     .message = "Test With Truncation",
	     // note: we test the full digest rather than the truncated one in this test
	     .expected = "\x4c\x1a\x03\x42\x4b\x55\xe0\x7f\xe7\xf2\x7b\xe1\xd5\x8b\xb9\x32\x4a\x9a\x5a\x04"sv},
	    {.key = std::string(80, '\xaa'),
	     .message = "Test Using Larger Than Block-Size Key - Hash Key First",
	     .expected = "\xaa\x4a\xe5\xe1\x52\x72\xd0\x0e\x95\x70\x56\x37\xce\x8a\x3b\x55\xed\x40\x21\x12"sv},
	    {.key = std::string(80, '\xaa'),
	     .message = "Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data",
	     .expected = "\xe8\xe9\x9d\x0f\x45\x23\x7d\x78\x6d\x6b\xba\xa7\x96\x5c\x78\x08\xbb\xff\x1a\x91"sv},
	};

	for (auto&& [key, message, expected] : testVectors) {
		auto actual = hmac("SHA1", key, message);
		BOOST_TEST(actual == expected, "expected " << expected << ", got: " << actual << "");
	}
}

struct TotpFixture
{
	uint64_t time;
	std::string_view expected;
};

BOOST_AUTO_TEST_CASE(test_totp)
{
	// test vectors from https://www.rfc-editor.org/rfc/rfc6238#appendix-B
	auto testVectors = std::vector<TotpFixture>{
	    {.time = 59, .expected = "94287082"},         {.time = 1111111109, .expected = "07081804"},
	    {.time = 1111111111, .expected = "14050471"}, {.time = 1234567890, .expected = "89005924"},
	    {.time = 2000000000, .expected = "69279037"}, {.time = 20000000000, .expected = "65353130"},
	};

	for (auto&& [time, expected] : testVectors) {
		auto actual = generateToken("12345678901234567890", time / 30u, 8);
		BOOST_TEST(actual == expected, "expected: " << expected << ", got: " << actual);
	}
}
