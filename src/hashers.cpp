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

#include <sodium.h>

#include "hashers.h"
#include "tools.h"

namespace hashers {

static std::array<std::unique_ptr<Hasher>, 3> hashers = {
	std::unique_ptr<Hasher>(new Argon2Hasher),
	std::unique_ptr<Hasher>(new ScryptSHA256Hasher),
	std::unique_ptr<Hasher>(new SHA1Hasher),
};

bool init()
{
	if (sodium_init() != 0) {
		return false;
	}

	std::cout << " found hashers:";
	for (const auto& hasher : hashers) {
		std::cout << " " << hasher->algorithm();
	}
	std::cout << std::endl;
	return true;
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

namespace {

static uint32_t circularShift(int bits, uint32_t value)
{
	return (value << bits) | (value >> (32 - bits));
}

static void processSHA1MessageBlock(const uint8_t* messageBlock, uint32_t* H)
{
	uint32_t W[80];
	for (int i = 0; i < 16; ++i) {
		const size_t offset = i << 2;
		W[i] = messageBlock[offset] << 24 | messageBlock[offset + 1] << 16 | messageBlock[offset + 2] << 8 | messageBlock[offset + 3];
	}

	for (int i = 16; i < 80; ++i) {
		W[i] = circularShift(1, W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]);
	}

	uint32_t A = H[0], B = H[1], C = H[2], D = H[3], E = H[4];

	for (int i = 0; i < 20; ++i) {
		const uint32_t tmp = circularShift(5, A) + ((B & C) | ((~B) & D)) + E + W[i] + 0x5A827999;
		E = D; D = C; C = circularShift(30, B); B = A; A = tmp;
	}

	for (int i = 20; i < 40; ++i) {
		const uint32_t tmp = circularShift(5, A) + (B ^ C ^ D) + E + W[i] + 0x6ED9EBA1;
		E = D; D = C; C = circularShift(30, B); B = A; A = tmp;
	}

	for (int i = 40; i < 60; ++i) {
		const uint32_t tmp = circularShift(5, A) + ((B & C) | (B & D) | (C & D)) + E + W[i] + 0x8F1BBCDC;
		E = D; D = C; C = circularShift(30, B); B = A; A = tmp;
	}

	for (int i = 60; i < 80; ++i) {
		const uint32_t tmp = circularShift(5, A) + (B ^ C ^ D) + E + W[i] + 0xCA62C1D6;
		E = D; D = C; C = circularShift(30, B); B = A; A = tmp;
	}

	H[0] += A;
	H[1] += B;
	H[2] += C;
	H[3] += D;
	H[4] += E;
}

}

std::string SHA1Hasher::encode(const std::string& input, const std::string&) const
{
	uint32_t H[] = {
		0x67452301,
		0xEFCDAB89,
		0x98BADCFE,
		0x10325476,
		0xC3D2E1F0
	};

	uint8_t messageBlock[64];
	size_t index = 0;

	uint32_t length_low = 0;
	uint32_t length_high = 0;
	for (char ch : input) {
		messageBlock[index++] = ch;

		length_low += 8;
		if (length_low == 0) {
			length_high++;
		}

		if (index == 64) {
			processSHA1MessageBlock(messageBlock, H);
			index = 0;
		}
	}

	messageBlock[index++] = 0x80;

	if (index > 56) {
		while (index < 64) {
			messageBlock[index++] = 0;
		}

		processSHA1MessageBlock(messageBlock, H);
		index = 0;
	}

	while (index < 56) {
		messageBlock[index++] = 0;
	}

	messageBlock[56] = length_high >> 24;
	messageBlock[57] = length_high >> 16;
	messageBlock[58] = length_high >> 8;
	messageBlock[59] = length_high;

	messageBlock[60] = length_low >> 24;
	messageBlock[61] = length_low >> 16;
	messageBlock[62] = length_low >> 8;
	messageBlock[63] = length_low;

	processSHA1MessageBlock(messageBlock, H);

	char hexstring[41];
	static const char hexDigits[] = {"0123456789abcdef"};
	for (int hashByte = 20; --hashByte >= 0;) {
		const uint8_t byte = H[hashByte >> 2] >> (((3 - hashByte) & 3) << 3);
		index = hashByte << 1;
		hexstring[index] = hexDigits[byte >> 4];
		hexstring[index + 1] = hexDigits[byte & 15];
	}
	return {hexstring, 40};
}

bool SHA1Hasher::verify(const std::string& input, const std::string& encoded) const
{
	return encode(input) == encoded;
}

const std::size_t Argon2Hasher::default_memory_cost = crypto_pwhash_argon2i_MEMLIMIT_INTERACTIVE;
const std::size_t Argon2Hasher::default_time_cost = crypto_pwhash_argon2i_OPSLIMIT_INTERACTIVE;

std::string Argon2Hasher::encode(const std::string& input, const std::string& salt) const
{
	return encode(input, salt, default_time_cost, default_memory_cost);
}

std::string Argon2Hasher::encode(const std::string& input, const std::string& salt, std::size_t timeCost, std::size_t memoryCost) const
{
	char out[crypto_pwhash_argon2i_STRBYTES];
	if (crypto_pwhash_argon2i_str(out, input.c_str(), input.size(), timeCost, memoryCost) == 0) {
		return algorithm() + out;
	}


	std::cout << "[Error - Argon2Hasher::encode] Out of memory." << std::endl;
	return {};
}

bool Argon2Hasher::verify(const std::string& input, const std::string& encoded) const
{
	StringVector split = explodeString(encoded, "$", 1);

	const std::string& algorithm = split[0];
	if (algorithm != this->algorithm()) {
		return false;
	}

	const std::string& hash = split[1];
	if (hash.size() != crypto_pwhash_argon2i_STRBYTES) {
		return false;
	}

	return crypto_pwhash_argon2i_str_verify(hash.c_str(), input.c_str(), input.size()) == 0;
}

const std::size_t ScryptSHA256Hasher::default_memory_cost = crypto_pwhash_scryptsalsa208sha256_MEMLIMIT_INTERACTIVE;
const std::size_t ScryptSHA256Hasher::default_time_cost = crypto_pwhash_scryptsalsa208sha256_OPSLIMIT_INTERACTIVE;

std::string ScryptSHA256Hasher::encode(const std::string& input, const std::string& salt) const
{
	return encode(input, salt, default_time_cost, default_memory_cost);
}

std::string ScryptSHA256Hasher::encode(const std::string& input, const std::string& salt, std::size_t timeCost, std::size_t memoryCost) const
{
	char out[crypto_pwhash_scryptsalsa208sha256_STRBYTES];
	if (crypto_pwhash_scryptsalsa208sha256_str(out, input.c_str(), input.size(), timeCost, memoryCost) == 0) {
		return algorithm() + out;
	}


	std::cout << "[Error - ScryptSHA256Hasher::encode] Out of memory." << std::endl;
	return {};
}

bool ScryptSHA256Hasher::verify(const std::string& input, const std::string& encoded) const
{
	StringVector split = explodeString(encoded, "$", 1);

	const std::string& algorithm = split[0];
	if (algorithm != this->algorithm()) {
		return false;
	}

	const std::string& hash = split[1];
	if (hash.size() != crypto_pwhash_scryptsalsa208sha256_STRBYTES) {
		return false;
	}

	return crypto_pwhash_scryptsalsa208sha256_str_verify(hash.c_str(), input.c_str(), input.size()) == 0;
}

}
