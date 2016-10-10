/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2016  Mark Samman <mark.samman@gmail.com>
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

struct NodeStruct;

typedef NodeStruct* NODE;

struct NodeStruct {
	uint32_t start = 0;
	uint32_t propsSize = 0;
	uint32_t type = 0;
	NodeStruct* next = nullptr;
	NodeStruct* child = nullptr;

	static void clearNet(NodeStruct* root) {
		if (root) {
			clearChild(root);
		}
	}

	private:
		static void clearNext(NodeStruct* node) {
			NodeStruct* deleteNode = node;
			NodeStruct* nextNode;

			while (deleteNode) {
				if (deleteNode->child) {
					clearChild(deleteNode->child);
				}

				nextNode = deleteNode->next;
				delete deleteNode;
				deleteNode = nextNode;
			}
		}

		static void clearChild(NodeStruct* node) {
			if (node->child) {
				clearChild(node->child);
			}

			if (node->next) {
				clearNext(node->next);
			}

			delete node;
		}
};

static constexpr auto NO_NODE = nullptr;

enum FILELOADER_ERRORS {
	ERROR_NONE,
	ERROR_INVALID_FILE_VERSION,
	ERROR_CAN_NOT_OPEN,
	ERROR_CAN_NOT_CREATE,
	ERROR_EOF,
	ERROR_SEEK_ERROR,
	ERROR_NOT_OPEN,
	ERROR_INVALID_NODE,
	ERROR_INVALID_FORMAT,
	ERROR_TELL_ERROR,
	ERROR_COULDNOTWRITE,
	ERROR_CACHE_ERROR,
};

class PropStream;

class FileLoader
{
	public:
		FileLoader() = default;
		~FileLoader();

		// non-copyable
		FileLoader(const FileLoader&) = delete;
		FileLoader& operator=(const FileLoader&) = delete;

		bool openFile(const char* filename, const char* identifier);
		const uint8_t* getProps(const NODE, size_t& size);
		bool getProps(const NODE, PropStream& props);
		NODE getChildNode(const NODE parent, uint32_t& type);
		NODE getNextNode(const NODE prev, uint32_t& type);

		FILELOADER_ERRORS getError() const {
			return lastError;
		}

	protected:
		enum SPECIAL_BYTES {
			ESCAPE_CHAR = 0xFD,
			NODE_START = 0xFE,
			NODE_END = 0xFF,
		};

		bool parseNode(NODE node);

		inline bool readByte(int32_t& value);
		inline bool readBytes(uint32_t size, int32_t pos);
		inline bool safeSeek(uint32_t pos);
		inline bool safeTell(int32_t& pos);

	protected:
		struct cache {
			uint8_t* data;
			uint32_t loaded;
			uint32_t base;
			uint32_t size;
		};

		static constexpr int32_t CACHE_BLOCKS = 3;
		cache cached_data[CACHE_BLOCKS] = {};

		uint8_t* buffer = new uint8_t[1024];
		NODE root = nullptr;
		FILE* file = nullptr;

		FILELOADER_ERRORS lastError = ERROR_NONE;
		uint32_t buffer_size = 1024;

		uint32_t cache_size = 0;
		static constexpr uint32_t NO_VALID_CACHE = std::numeric_limits<uint32_t>::max();
		uint32_t cache_index = NO_VALID_CACHE;
		uint32_t cache_offset = NO_VALID_CACHE;

		inline uint32_t getCacheBlock(uint32_t pos);
		int32_t loadCacheBlock(uint32_t pos);
};

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

	protected:
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

	protected:
		std::vector<char> buffer;
};

#endif
