/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2017  Mark Samman <mark.samman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "otpch.h"

#include <cryptopp/hex.h>
#include <cryptopp/sha.h>

#include "hashers.h"

namespace hashers {

namespace {

std::unique_ptr<Hasher> hashers[] = {
		std::unique_ptr<Hasher>(new SHA1Hasher)
};

std::string hexencode(const byte* raw, std::size_t length)
{
	std::string output;
	CryptoPP::HexEncoder encoder{new CryptoPP::StringSink{output}, false};
	encoder.Put(raw, length);
	encoder.MessageEnd();
	return output;
}

}

const Hasher& getHasher(const std::string& algorithm)
{
	for (const auto& hasher : hashers) {
		if (hasher->algorithm() == algorithm) {
			return *hasher;
		}
	}

	throw std::invalid_argument("[Error - getHasher] Unknown hasher specified: " + algorithm);
}

const Hasher& identifyHasher(const std::string& encoded)
{
	auto it = encoded.find('$');

	// ancient versions of TFS used plain SHA1 passwords
	if (encoded.size() == 40 && it == std::string::npos) {
		return getHasher("sha1");
	}

	return getHasher(encoded.substr(0, it));
}

std::string SHA1Hasher::encode(const std::string& input, const std::string&) const
{
	CryptoPP::SHA1 hasher;
	byte digest[CryptoPP::SHA1::DIGESTSIZE];
	hasher.CalculateDigest(digest, reinterpret_cast<const byte*>(input.c_str()), input.size());
	return hexencode(digest, sizeof(digest));
}

bool SHA1Hasher::verify(const std::string& input, const std::string& encoded) const
{
	return encode(input) == encoded;
}

}
