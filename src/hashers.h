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

#ifndef TFS_HASHERS_H
#define TFS_HASHERS_H

#include <string>

namespace hashers {

struct Hasher {
	virtual std::string algorithm() const = 0;
	virtual std::string encode(const std::string& input, const std::string& salt) const = 0;
	virtual bool verify(const std::string& input, const std::string& encoded) const = 0;
};

const Hasher& getHasher(const std::string& algorithm);
const Hasher& identifyHasher(const std::string& encoded);

struct SHA1Hasher : public Hasher {
	std::string algorithm() const override { return "deprecated_sha1"; }
	std::string encode(const std::string& input, const std::string& salt = "") const override;
	bool verify(const std::string& input, const std::string& encoded) const override;
};

struct PBKDF2Hasher : public Hasher {
	std::string algorithm() const override { return "pbkdf2_sha256"; }
	std::string encode(const std::string& input, const std::string& salt) const override;
	std::string encode(const std::string& input, const std::string& salt, std::size_t iterations) const;
	bool verify(const std::string& input, const std::string& encoded) const override;

	static constexpr std::size_t default_iterations = 30000u;
};

}

#endif //TFS_HASHERS_H
