// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

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

	auto headerIndex = key.find(header);
	if (headerIndex == std::string::npos) {
		throw std::runtime_error("Missing RSA private key PEM header.");
	}

	auto footerIndex = key.find(footer, headerIndex + 1);
	if (footerIndex == std::string::npos) {
		throw std::runtime_error("Missing RSA private key PEM footer.");
	}

	key = key.substr(headerIndex + header.size(), footerIndex - headerIndex - header.size());

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
