// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "rsa.h"

#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

namespace {

struct Deleter
{
	void operator()(EVP_PKEY* pkey) const { EVP_PKEY_free(pkey); }
	void operator()(EVP_PKEY_CTX* ctx) const { EVP_PKEY_CTX_free(ctx); }
	void operator()(FILE* fp) const { fclose(fp); }
};

template <class T>
using C_ptr = std::unique_ptr<T, Deleter>;

C_ptr<EVP_PKEY> pkey = nullptr;

} // namespace

namespace tfs::rsa {

void decrypt(uint8_t* msg, size_t len)
{
	C_ptr<EVP_PKEY_CTX> pctx{EVP_PKEY_CTX_new_from_pkey(nullptr, pkey.get(), nullptr)};
	EVP_PKEY_decrypt_init(pctx.get());
	EVP_PKEY_CTX_set_rsa_padding(pctx.get(), RSA_NO_PADDING);

	EVP_PKEY_decrypt(pctx.get(), msg, &len, msg, len);
}

EVP_PKEY* loadPEM(std::string_view filename)
{
	C_ptr<FILE> fp{fopen(filename.data(), "r")};
	if (!fp) {
		throw std::runtime_error(fmt::format("Error while reading {}: {}", filename, strerror(errno)));
	}

	EVP_PKEY* pkey_ = nullptr;
	if (!PEM_read_PrivateKey(fp.get(), &pkey_, nullptr, nullptr)) {
		throw std::runtime_error(
		    fmt::format("Error while reading private key: {}", ERR_error_string(ERR_get_error(), nullptr)));
	}

	pkey.reset(pkey_);
	return pkey_;
}

} // namespace tfs::rsa
