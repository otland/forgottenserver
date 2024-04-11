// Copyright 2024 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_RSA_H
#define FS_RSA_H

#include <openssl/evp.h>

namespace tfs {

class RSA
{
public:
	static constexpr int16_t BUFFER_LENGTH = 128;

	RSA() = default;

	void loadPEM(std::string_view filename);
	void decrypt(uint8_t* msg) const;

private:
	std::unique_ptr<EVP_PKEY, decltype(&EVP_PKEY_free)> pkey{nullptr, EVP_PKEY_free};
};

} // namespace tfs

#endif // FS_RSA_H
