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

#ifndef FS_FILELOADER_H_9B663D19E58D42E6BFACFE5B09D7A05E
#define FS_FILELOADER_H_9B663D19E58D42E6BFACFE5B09D7A05E

#include <limits>
#include <vector>
#include <boost/iostreams/device/mapped_file.hpp>

class PropStream;

namespace OTB {
using MappedFile = boost::iostreams::mapped_file_source;
using ContentIt  = MappedFile::iterator;
using Identifier = std::array<char, 4>;

struct Node
{
	using ChildrenVector = std::vector<Node>;

	ChildrenVector children;
	ContentIt      propsBegin;
	ContentIt      propsEnd;
	uint8_t           type;
	enum NodeChar: uint8_t
	{
		ESCAPE = 0xFD,
		START  = 0xFE,
		END    = 0xFF,
	};
};

struct LoadError : std::exception {
	const char* what() const noexcept override = 0;
};

struct InvalidOTBFormat final : LoadError {
	const char* what() const noexcept override {
		return "Invalid OTBM file format";
	}
};

class Loader {
	MappedFile     fileContents;
	Node              root;
	std::vector<char> propBuffer;
public:
	Loader(const std::string& fileName, const Identifier& acceptedIdentifier);
	bool getProps(const Node& node, PropStream& props);
	const Node& parseTree();
};

} //namespace OTB

class PropStream
{
	public:
		void init(const char* a, size_t size) {
			p = a;
			end = a + size;
		}

		size_t size() const {
			return end - p;
		}

		template <typename T>
		bool read(T& ret) {
			if (size() < sizeof(T)) {
				return false;
			}

			memcpy(&ret, p, sizeof(T));
			p += sizeof(T);
			return true;
		}

		bool readString(std::string& ret) {
			uint16_t strLen;
			if (!read<uint16_t>(strLen)) {
				return false;
			}

			if (size() < strLen) {
				return false;
			}

			char* str = new char[strLen + 1];
			memcpy(str, p, strLen);
			str[strLen] = 0;
			ret.assign(str, strLen);
			delete[] str;
			p += strLen;
			return true;
		}

		bool skip(size_t n) {
			if (size() < n) {
				return false;
			}

			p += n;
			return true;
		}

	private:
		const char* p = nullptr;
		const char* end = nullptr;
};

class PropWriteStream
{
	public:
		PropWriteStream() = default;

		// non-copyable
		PropWriteStream(const PropWriteStream&) = delete;
		PropWriteStream& operator=(const PropWriteStream&) = delete;

		const char* getStream(size_t& size) const {
			size = buffer.size();
			return buffer.data();
		}

		void clear() {
			buffer.clear();
		}

		template <typename T>
		void write(T add) {
			char* addr = reinterpret_cast<char*>(&add);
			std::copy(addr, addr + sizeof(T), std::back_inserter(buffer));
		}

		void writeString(const std::string& str) {
			size_t strLength = str.size();
			if (strLength > std::numeric_limits<uint16_t>::max()) {
				write<uint16_t>(0);
				return;
			}

			write(static_cast<uint16_t>(strLength));
			std::copy(str.begin(), str.end(), std::back_inserter(buffer));
		}

	private:
		std::vector<char> buffer;
};

#endif
