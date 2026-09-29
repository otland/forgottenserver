#define BOOST_TEST_MODULE proxyprotocol

#include "../otpch.h"

#include "../proxyprotocol.h"

#include <boost/test/unit_test.hpp>

namespace proxy_protocol = tfs::net::proxy_protocol;

namespace {

constexpr uint8_t LOCAL = 0x0, PROXY = 0x1;
constexpr uint8_t UNSPEC = 0x0, INET = 0x1, INET6 = 0x2, UNIX = 0x3;
constexpr uint8_t STREAM = 0x1;

// Builds the fixed-size part of a v2 header: signature, version and command, family and transport, big-endian length
std::vector<uint8_t> makeHeader(uint8_t command, uint8_t family, uint16_t length)
{
	std::vector<uint8_t> header{proxy_protocol::SIGNATURE.begin(), proxy_protocol::SIGNATURE.end()};
	header.push_back(0x20 | command);
	header.push_back((family << 4) | STREAM);
	header.push_back(length >> 8);
	header.push_back(length & 0xFF);
	return header;
}

} // namespace

BOOST_AUTO_TEST_CASE(test_matches_signature_partial_and_full)
{
	const auto& signature = proxy_protocol::SIGNATURE;
	BOOST_TEST(proxy_protocol::matchesSignature(signature.data(), 2));
	BOOST_TEST(proxy_protocol::matchesSignature(signature.data(), signature.size()));
	// extra bytes beyond the signature are not compared
	BOOST_TEST(proxy_protocol::matchesSignature(makeHeader(PROXY, INET, 12).data(), proxy_protocol::HEADER_LENGTH));

	// a regular packet whose length header happens to be 0x0D 0x0A passes the two-byte probe but not the full check
	const std::array<uint8_t, proxy_protocol::HEADER_LENGTH> packet = {0x0D, 0x0A, 0x91, 0x6A, 0xC1, 0x0D, 0x0A, 0x00,
	                                                                   0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
	BOOST_TEST(proxy_protocol::matchesSignature(packet.data(), 2));
	BOOST_TEST(!proxy_protocol::matchesSignature(packet.data(), packet.size()));
}

BOOST_AUTO_TEST_CASE(test_parse_header_local)
{
	auto header = proxy_protocol::parseHeader(makeHeader(LOCAL, UNSPEC, 0).data());
	BOOST_REQUIRE(header);
	BOOST_TEST((header->command == proxy_protocol::Command::LOCAL));
	BOOST_TEST(header->length == 0);

	// a LOCAL header may still carry an address block, which is skipped
	header = proxy_protocol::parseHeader(makeHeader(LOCAL, INET, 12).data());
	BOOST_REQUIRE(header);
	BOOST_TEST((header->command == proxy_protocol::Command::LOCAL));
	BOOST_TEST(header->length == 12);
}

BOOST_AUTO_TEST_CASE(test_parse_header_proxy_inet)
{
	auto header = proxy_protocol::parseHeader(makeHeader(PROXY, INET, 12).data());
	BOOST_REQUIRE(header);
	BOOST_TEST((header->command == proxy_protocol::Command::PROXY));
	BOOST_TEST((header->family == proxy_protocol::AddressFamily::INET));
	BOOST_TEST(header->length == 12);

	header = proxy_protocol::parseHeader(makeHeader(PROXY, INET6, 36).data());
	BOOST_REQUIRE(header);
	BOOST_TEST((header->family == proxy_protocol::AddressFamily::INET6));
	BOOST_TEST(header->length == 36);

	// TLVs may follow the address block
	header = proxy_protocol::parseHeader(makeHeader(PROXY, INET, 12 + 7).data());
	BOOST_REQUIRE(header);
	BOOST_TEST(header->length == 19);
}

BOOST_AUTO_TEST_CASE(test_parse_header_rejects_families_without_address)
{
	// neither UNSPEC nor UNIX yields a client address, accepting them would mark the connection as relayed while
	// keeping the proxy's own address
	BOOST_TEST(!proxy_protocol::parseHeader(makeHeader(PROXY, UNSPEC, 0).data()));
	BOOST_TEST(!proxy_protocol::parseHeader(makeHeader(PROXY, UNSPEC, 12).data()));
	BOOST_TEST(!proxy_protocol::parseHeader(makeHeader(PROXY, UNIX, 216).data()));
}

BOOST_AUTO_TEST_CASE(test_parse_header_rejects_malformed)
{
	// address block shorter than the family requires
	BOOST_TEST(!proxy_protocol::parseHeader(makeHeader(PROXY, INET, 11).data()));
	BOOST_TEST(!proxy_protocol::parseHeader(makeHeader(PROXY, INET6, 35).data()));

	// wrong version
	auto header = makeHeader(PROXY, INET, 12);
	header[12] = 0x11;
	BOOST_TEST(!proxy_protocol::parseHeader(header.data()));

	// unknown command
	header = makeHeader(PROXY, INET, 12);
	header[12] = 0x22;
	BOOST_TEST(!proxy_protocol::parseHeader(header.data()));

	// unknown family
	header = makeHeader(PROXY, INET, 12);
	header[13] = 0x41;
	BOOST_TEST(!proxy_protocol::parseHeader(header.data()));

	// corrupted signature
	header = makeHeader(PROXY, INET, 12);
	header[5] = 0x00;
	BOOST_TEST(!proxy_protocol::parseHeader(header.data()));
}

BOOST_AUTO_TEST_CASE(test_parse_source_address)
{
	// source address, destination address, source port, destination port
	const std::vector<uint8_t> inet = {192, 168, 1, 2, 10, 0, 0, 1, 0x1F, 0x90, 0x1C, 0x36};
	auto header = proxy_protocol::parseHeader(makeHeader(PROXY, INET, inet.size()).data());
	BOOST_REQUIRE(header);
	auto address = proxy_protocol::parseSourceAddress(*header, inet.data());
	BOOST_REQUIRE(address);
	BOOST_TEST(address->to_string() == "192.168.1.2");

	std::vector<uint8_t> inet6(36, 0);
	inet6[0] = 0x20, inet6[1] = 0x01, inet6[2] = 0x0D, inet6[3] = 0xB8, inet6[15] = 0x01;
	header = proxy_protocol::parseHeader(makeHeader(PROXY, INET6, inet6.size()).data());
	BOOST_REQUIRE(header);
	address = proxy_protocol::parseSourceAddress(*header, inet6.data());
	BOOST_REQUIRE(address);
	BOOST_TEST(address->to_string() == "2001:db8::1");

	// a LOCAL command carries no client address even if an address block follows
	header = proxy_protocol::parseHeader(makeHeader(LOCAL, INET, inet.size()).data());
	BOOST_REQUIRE(header);
	BOOST_TEST(!proxy_protocol::parseSourceAddress(*header, inet.data()));
}

BOOST_AUTO_TEST_CASE(test_is_trusted_peer)
{
	using boost::asio::ip::make_address;
	BOOST_TEST(proxy_protocol::isTrustedPeer(make_address("127.0.0.1")));
	BOOST_TEST(proxy_protocol::isTrustedPeer(make_address("127.0.0.53")));
	BOOST_TEST(proxy_protocol::isTrustedPeer(make_address("::1")));
	BOOST_TEST(proxy_protocol::isTrustedPeer(make_address("::ffff:127.0.0.1")));
	BOOST_TEST(!proxy_protocol::isTrustedPeer(make_address("::ffff:10.0.0.1")));
	BOOST_TEST(!proxy_protocol::isTrustedPeer(make_address("10.0.0.1")));
	BOOST_TEST(!proxy_protocol::isTrustedPeer(make_address("2001:db8::1")));
}
