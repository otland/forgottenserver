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

#include <cryptopp/base64.h>
#include <cryptopp/hex.h>
#include <cryptopp/pwdbased.h>
#include <cryptopp/sha.h>

#include "hashers.h"
#include "tools.h"

namespace hashers {

static std::array<std::unique_ptr<Hasher>, 2> hashers = {
	std::unique_ptr<Hasher>(new PBKDF2Hasher),
	std::unique_ptr<Hasher>(new SHA1Hasher),
};

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

	std::string output;
	CryptoPP::HexEncoder encoder{new CryptoPP::StringSink{output}, false};
	encoder.Put(digest, sizeof(digest));
	encoder.MessageEnd();
	return output;
}

bool SHA1Hasher::verify(const std::string& input, const std::string& encoded) const
{
	return encode(input) == encoded;
}

std::string PBKDF2Hasher::encode(const std::string& input, const std::string& salt) const
{
	return encode(input, salt, default_iterations);
}

std::string PBKDF2Hasher::encode(const std::string& input, const std::string& salt, std::size_t iterations) const
{
	CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA256> hasher;
	byte digest[CryptoPP::SHA256::DIGESTSIZE];
	hasher.DeriveKey(digest, sizeof(digest), 0, reinterpret_cast<const byte*>(input.c_str()), input.size(), reinterpret_cast<const byte*>(salt.c_str()), salt.size(), iterations);

	std::string output;
	CryptoPP::Base64Encoder encoder{new CryptoPP::StringSink{output}, false};
	encoder.Put(digest, sizeof(digest));
	encoder.MessageEnd();
	return output;
}

bool PBKDF2Hasher::verify(const std::string& input, const std::string& encoded) const
{
	StringVector split = explodeString(encoded, "$", 3);

	const std::string& algorithm = split[0];
	if (algorithm != this->algorithm()) {
		return false;
	}

	std::size_t iterations = std::strtoull(split[1].c_str(), nullptr, 10);
	const std::string& salt = split[2];
	const std::string& hash = split[3];

	return encode(input, salt, iterations) == hash;
}

}
