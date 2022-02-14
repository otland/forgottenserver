/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2019  Mark Samman <mark.samman@gmail.com>
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

#include "rsa.h"

#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>
#include <fmt/color.h>

#include <fstream>
#include <sstream>

static CryptoPP::AutoSeededRandomPool prng;

void RSA::decrypt(char* msg) const
{
	try {
		CryptoPP::Integer m{reinterpret_cast<uint8_t*>(msg), 128};
		auto c = pk.CalculateInverse(prng, m);
		c.Encode(reinterpret_cast<uint8_t*>(msg), 128);
	} catch (const CryptoPP::Exception& e) {
		fmt::print(fg(fmt::color::crimson) | fmt::emphasis::bold, e.what(), "\n");
	}
}

static const std::string header = "-----BEGIN RSA PRIVATE KEY-----";
static const std::string footer = "-----END RSA PRIVATE KEY-----";

void RSA::loadPEM(const std::string& filename)
{
	std::ifstream file{filename};

	if (!file.is_open()) {
		throw std::runtime_error("Missing file " + filename + ".");
	}

	std::ostringstream oss;
	for (std::string line; std::getline(file, line); oss << line);
	std::string key = oss.str();

	auto pos1 = key.find(header);
	if (pos1 == std::string::npos) {
		throw std::runtime_error("Missing RSA private key PEM header.");
	}

	auto pos2 = key.find(footer, pos1 + 1);
	if (pos2 == std::string::npos) {
		throw std::runtime_error("Missing RSA private key PEM footer.");
	}

	key = key.substr(pos1 + header.size(), pos2 - pos1 - header.size());

	CryptoPP::ByteQueue queue;
	CryptoPP::Base64Decoder decoder;
	decoder.Attach(new CryptoPP::Redirector(queue));
	decoder.Put(reinterpret_cast<const uint8_t*>(key.c_str()), key.size());
	decoder.MessageEnd();

	pk.BERDecodePrivateKey(queue, false, queue.MaxRetrievable());

	if (!pk.Validate(prng, 3)) {
		throw std::runtime_error("RSA private key is not valid.");
	}
}
