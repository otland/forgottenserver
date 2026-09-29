// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "proxyprotocol.h"

#include <algorithm>
#include <cstring>

namespace tfs::net::proxy_protocol {

namespace {

constexpr uint8_t VERSION = 0x2;

// Size of the address block (addresses + ports) for each family
constexpr size_t addressBlockLength(AddressFamily family)
{
	switch (family) {
		case AddressFamily::INET:
			return 4 + 4 + 2 + 2;
		case AddressFamily::INET6:
			return 16 + 16 + 2 + 2;
		default:
			return 0;
	}
}

} // namespace

bool isTrustedPeer(const boost::asio::ip::address& peer)
{
	if (peer.is_v6()) {
		const auto bytes = peer.to_v6().to_bytes();
		const bool v4Mapped = std::all_of(bytes.begin(), bytes.begin() + 10, [](uint8_t byte) { return byte == 0; }) &&
		                      bytes[10] == 0xFF && bytes[11] == 0xFF;
		if (v4Mapped) {
			return boost::asio::ip::address_v4{{bytes[12], bytes[13], bytes[14], bytes[15]}}.is_loopback();
		}
	}
	return peer.is_loopback();
}

bool matchesSignature(const uint8_t* data, size_t size)
{
	size = std::min(size, SIGNATURE.size());
	return std::equal(data, data + size, SIGNATURE.begin());
}

std::optional<Header> parseHeader(const uint8_t* data)
{
	if (!matchesSignature(data, SIGNATURE.size())) {
		return std::nullopt;
	}

	const uint8_t versionCommand = data[12];
	if ((versionCommand >> 4) != VERSION) {
		return std::nullopt;
	}

	const uint8_t command = versionCommand & 0x0F;
	if (command > static_cast<uint8_t>(Command::PROXY)) {
		return std::nullopt;
	}

	// low nibble is the transport protocol, which is irrelevant here
	const uint8_t family = data[13] >> 4;
	if (family > static_cast<uint8_t>(AddressFamily::UNIX)) {
		return std::nullopt;
	}

	Header header{
	    .command = static_cast<Command>(command),
	    .family = static_cast<AddressFamily>(family),
	    .length = static_cast<uint16_t>((data[14] << 8) | data[15]),
	};

	if (header.command == Command::PROXY) {
		// Only INET and INET6 carry a usable source address. The specification allows UNSPEC (falling back to the
		// socket address) but a local proxy relaying a client it cannot identify is a misconfiguration, and accepting
		// it would mark the connection as relayed while the loopback address stays in effect for bans and limits
		if (header.family != AddressFamily::INET && header.family != AddressFamily::INET6) {
			return std::nullopt;
		}

		if (header.length < addressBlockLength(header.family)) {
			return std::nullopt;
		}
	}
	return header;
}

std::optional<boost::asio::ip::address> parseSourceAddress(const Header& header, const uint8_t* data)
{
	if (header.command != Command::PROXY) {
		return std::nullopt;
	}

	// The address block starts with the source address, in network byte order
	switch (header.family) {
		case AddressFamily::INET: {
			boost::asio::ip::address_v4::bytes_type bytes;
			std::memcpy(bytes.data(), data, bytes.size());
			return boost::asio::ip::address_v4{bytes};
		}
		case AddressFamily::INET6: {
			boost::asio::ip::address_v6::bytes_type bytes;
			std::memcpy(bytes.data(), data, bytes.size());
			return boost::asio::ip::address_v6{bytes};
		}
		default:
			return std::nullopt;
	}
}

} // namespace tfs::net::proxy_protocol
