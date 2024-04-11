// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "rsa.h"

#include <fstream>
#include <openssl/err.h>
#include <openssl/pem.h>

namespace tfs {

void RSA::decrypt(uint8_t* msg) const
{
	std::unique_ptr<EVP_PKEY_CTX, decltype(&EVP_PKEY_CTX_free)> pctx{
	    EVP_PKEY_CTX_new_from_pkey(nullptr, pkey.get(), nullptr), EVP_PKEY_CTX_free};
	EVP_PKEY_decrypt_init(pctx.get());

	size_t len = tfs::RSA::BUFFER_LENGTH;
	EVP_PKEY_decrypt(pctx.get(), msg, &len, msg, len);

	fmt::print(fg(fmt::color::crimson) | fmt::emphasis::bold, "\n");
}

void RSA::loadPEM(std::string_view filename)
{
	std::unique_ptr<FILE, decltype(&fclose)> fp{fopen(filename.data(), "r"), fclose};
	if (!fp) {
		throw std::runtime_error(fmt::format("Error while reading {}: {}", filename, strerror(errno)));
	}

	EVP_PKEY* pkey;
	if (!PEM_read_PrivateKey(fp.get(), &pkey, nullptr, nullptr)) {
		throw std::runtime_error(
		    fmt::format("Error while reading private key: {}", ERR_error_string(ERR_get_error(), nullptr)));
	}

	this->pkey.reset(pkey);
}

} // namespace tfs
