// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_PROXYPROTOCOL_H
#define FS_PROXYPROTOCOL_H

#include <array>
#include <boost/asio/ip/address.hpp>
#include <cstddef>
#include <cstdint>
#include <optional>

// Parser for the PROXY protocol, version 2 (binary), as sent by HAProxy's `send-proxy-v2` and compatible proxies:
// https://www.haproxy.org/download/2.9/doc/proxy-protocol.txt
//
// A v2 header consists of a fixed 16-byte part (12-byte signature, version/command byte, address family/transport
// byte, big-endian 16-bit length) followed by `length` bytes holding the address block and optional TLVs.
namespace tfs::net::proxy_protocol {

// Whether a PROXY protocol header from the given peer is trusted: only proxies running on the same host are, which
// includes IPv4 loopback peers reported as IPv4-mapped IPv6 addresses (::ffff:127.0.0.1) by dual-stack listeners
bool isTrustedPeer(const boost::asio::ip::address& peer);

inline constexpr size_t HEADER_LENGTH = 16;

inline constexpr std::array<uint8_t, 12> SIGNATURE = {0x0D, 0x0A, 0x0D, 0x0A, 0x00, 0x0D,
                                                      0x0A, 0x51, 0x55, 0x49, 0x54, 0x0A};

enum class Command : uint8_t
{
	LOCAL = 0x0, // connection established by the proxy itself (e.g. health check); no address information
	PROXY = 0x1, // connection relayed on behalf of a client whose address follows
};

enum class AddressFamily : uint8_t
{
	UNSPEC = 0x0,
	INET = 0x1,
	INET6 = 0x2,
	UNIX = 0x3,
};

struct Header
{
	Command command;
	AddressFamily family;
	uint16_t length; // number of bytes following the fixed-size header (address block + TLVs)
};

// Returns whether the first `size` bytes of `data` are consistent with the start of a v2 header. Compares at most
// SIGNATURE.size() bytes, so it can be used on a partial read to decide whether to keep reading a PROXY header.
bool matchesSignature(const uint8_t* data, size_t size);

// Parses the fixed-size header (HEADER_LENGTH bytes). Returns nullopt if the header is malformed, including when a
// PROXY command announces a family other than INET or INET6, or a length too short to hold its address block.
std::optional<Header> parseHeader(const uint8_t* data);

// Returns the original client address from the address block following the header (`header.length` bytes), or
// nullopt if the header carries no address (LOCAL command).
std::optional<boost::asio::ip::address> parseSourceAddress(const Header& header, const uint8_t* data);

} // namespace tfs::net::proxy_protocol

#endif // FS_PROXYPROTOCOL_H
