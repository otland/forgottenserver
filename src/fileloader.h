// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_FILELOADER_H_9B663D19E58D42E6BFACFE5B09D7A05E
#define FS_FILELOADER_H_9B663D19E58D42E6BFACFE5B09D7A05E

#include <filesystem>
#include <fstream>
#include <limits>
#include <vector>

class PropStream;

namespace OTB {

class MappedFile
{
public:
    using container_type = std::vector<char>;
    using iterator = container_type::const_iterator;

    MappedFile() = default;

    explicit MappedFile(const std::filesystem::path& path)
    {
        open(path);
    }

    bool open(const std::filesystem::path& path)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file) {
            return false;
        }

        data_ = {
            std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>()
        };

        return true;
    }

    [[nodiscard]]
    bool is_open() const noexcept
    {
        return !data_.empty();
    }

    [[nodiscard]]
    iterator begin() const noexcept
    {
        return data_.begin();
    }

    [[nodiscard]]
    iterator end() const noexcept
    {
        return data_.end();
    }

    [[nodiscard]]
    const char* data() const noexcept
    {
        return data_.data();
    }

    [[nodiscard]]
    std::size_t size() const noexcept
    {
        return data_.size();
    }

private:
    container_type data_;
};

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
