/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2015  Mark Samman <mark.samman@gmail.com>
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

struct NodeStruct;

typedef NodeStruct* NODE;

struct NodeStruct {
	NodeStruct() : start(0), propsSize(0), type(0), next(nullptr), child(nullptr) {}

	uint32_t start;
	uint32_t propsSize;
	uint32_t type;
	NodeStruct* next;
	NodeStruct* child;

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

#define NO_NODE 0

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
		FileLoader();
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
			return m_lastError;
		}

	protected:
		enum SPECIAL_BYTES {
			NODE_START = 0xFE,
			NODE_END = 0xFF,
			ESCAPE_CHAR = 0xFD,
		};

		bool parseNode(NODE node);

		inline bool readByte(int32_t& value);
		inline bool readBytes(uint8_t* buffer, uint32_t size, int32_t pos);
		inline bool safeSeek(uint32_t pos);
		inline bool safeTell(int32_t& pos);

	protected:
		struct _cache {
			uint8_t* data;
			uint32_t loaded;
			uint32_t base;
			uint32_t size;
		};

#define CACHE_BLOCKS 3
		_cache m_cached_data[CACHE_BLOCKS];

		uint8_t* m_buffer;
		NODE m_root;
		FILE* m_file;

		FILELOADER_ERRORS m_lastError;
		uint32_t m_buffer_size;

		uint32_t m_cache_size;
#define NO_VALID_CACHE 0xFFFFFFFF
		uint32_t m_cache_index;
		uint32_t m_cache_offset;

		inline uint32_t getCacheBlock(uint32_t pos);
		int32_t loadCacheBlock(uint32_t pos);
};

class PropStream
{
	public:
		PropStream() {
			end = nullptr;
			p = nullptr;
		}

		void init(const char* a, size_t size) {
			p = a;
			end = a + size;
		}

		size_t size() const {
			return end - p;
		}

		template <typename T>
		inline bool readStruct(T*& ret) {
			if (size() < sizeof(T)) {
				ret = nullptr;
				return false;
			}

			ret = reinterpret_cast<const T*>(p);
			p += sizeof(T);
			return true;
		}

		template <typename T>
		inline bool read(T& ret) {
			if (size() < sizeof(T)) {
				return false;
			}

			ret = *reinterpret_cast<const T*>(p);
			p += sizeof(T);
			return true;
		}

		inline bool readString(std::string& ret) {
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

		inline bool skip(size_t n) {
			if (size() < n) {
				return false;
			}

			p += n;
			return true;
		}

	protected:
		const char* p;
		const char* end;
};

class PropWriteStream
{
	public:
		PropWriteStream() {
			buffer_size = 32;
			buffer = reinterpret_cast<char*>(malloc(buffer_size));
			if (!buffer) {
				throw std::bad_alloc();
			}

			size = 0;
		}

		~PropWriteStream() {
			free(buffer);
		}

		// non-copyable
		PropWriteStream(const PropWriteStream&) = delete;
		PropWriteStream& operator=(const PropWriteStream&) = delete;

		const char* getStream(size_t& _size) const {
			_size = size;
			return buffer;
		}

		inline void clear() {
			size = 0;
		}

		template <typename T>
		inline void write(T add) {
			reserve(sizeof(T));
			memcpy(&buffer[size], &add, sizeof(T));
			size += sizeof(T);
		}

		inline void writeString(const std::string& str) {
			size_t str_len = str.size();
			write<uint16_t>(str_len);
			reserve(str_len);
			memcpy(&buffer[size], str.c_str(), str_len);
			size += str_len;
		}

	protected:
		void reserve(size_t length) {
			if ((buffer_size - size) >= length) {
				return;
			}

			do {
				buffer_size <<= 1;
			} while ((buffer_size - size) < length);

			void* newBuffer = realloc(buffer, buffer_size);
			if (!newBuffer) {
				throw std::bad_alloc();
			}

			buffer = reinterpret_cast<char*>(newBuffer);
		}

		char* buffer;
		size_t buffer_size;
		size_t size;
};

#endif
