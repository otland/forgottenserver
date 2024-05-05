// Copyright 2024 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_RSA_H
#define FS_RSA_H

struct evp_pkey_st;
typedef struct evp_pkey_st EVP_PKEY;

namespace tfs::rsa {

EVP_PKEY* loadPEM(std::string_view pem);
void decrypt(uint8_t* msg, size_t len);

} // namespace tfs::rsa

#endif // FS_RSA_H
