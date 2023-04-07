// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_RSA_H
#define FS_RSA_H

class RSA
{
public:
	RSA() = default;

	// non-copyable
	RSA(const RSA&) = delete;
	RSA& operator=(const RSA&) = delete;

	void loadPEM(const std::string& filename);
	void decrypt(char* msg) const;

private:
	CryptoPP::RSA::PrivateKey pk;
};

#endif // FS_RSA_H
