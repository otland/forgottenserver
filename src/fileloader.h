/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2013  Mark Samman <mark.samman@gmail.com>
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
		NodeStruct() {
			start = propsSize = type = 0;
			next = child = 0;
		}

		virtual ~NodeStruct() {}

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
		virtual ~FileLoader();

		bool openFile(const char* filename, const char* identifier, bool write, bool caching = false);
		const uint8_t* getProps(const NODE, uint32_t& size);
		bool getProps(const NODE, PropStream& props);
		NODE getChildNode(const NODE parent, uint32_t& type);
		NODE getNextNode(const NODE prev, uint32_t& type);

		int32_t getError() const {
			return m_lastError;
		}
		void clearError() {
			m_lastError = ERROR_NONE;
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

	public:
		inline bool writeData(const void* data, int32_t size, bool unescape) {
			for (int32_t i = 0; i < size; ++i) {
				uint8_t c = *(((uint8_t*)data) + i);

				if (unescape && (c == NODE_START || c == NODE_END || c == ESCAPE_CHAR)) {
					uint8_t escape = ESCAPE_CHAR;
					size_t value = fwrite(&escape, 1, 1, m_file);
					if (value != 1) {
						m_lastError = ERROR_COULDNOTWRITE;
						return false;
					}
				}

				size_t value = fwrite(&c, 1, 1, m_file);
				if (value != 1) {
					m_lastError = ERROR_COULDNOTWRITE;
					return false;
				}
			}

			return true;
		}

	protected:
		FILE* m_file;
		FILELOADER_ERRORS m_lastError;
		NODE m_root;
		uint32_t m_buffer_size;
		uint8_t* m_buffer;

		bool m_use_cache;
		struct _cache {
			uint32_t loaded;
			uint32_t base;
			uint32_t size;
			uint8_t* data;
		};

#define CACHE_BLOCKS 3
		uint32_t m_cache_size;
		_cache m_cached_data[CACHE_BLOCKS];
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
		~PropStream() {}

		void init(const char* a, uint32_t size) {
			p = a;
			end = a + size;
		}

		size_t size() const {
			return end - p;
		}

		template <typename T>
		inline bool GET_STRUCT(T* &ret) {
			if (size() < (long)sizeof(T)) {
				ret = nullptr;
				return false;
			}

			ret = (T*)p;
			p += sizeof(T);
			return true;
		}

		template <typename T>
		inline bool GET_VALUE(T& ret) {
			if (size() < (long)sizeof(T)) {
				return false;
			}

			ret = *((T*)p);
			p += sizeof(T);
			return true;
		}

		inline bool GET_TIME(time_t& ret) {
			return GET_VALUE(ret);
		}

		inline bool GET_ULONG(uint32_t& ret) {
			return GET_VALUE(ret);
		}

		inline bool GET_USHORT(uint16_t& ret) {
			return GET_VALUE(ret);
		}

		inline bool GET_UCHAR(uint8_t& ret) {
			return GET_VALUE(ret);
		}

		inline bool GET_STRING(std::string& ret) {
			uint16_t str_len;
			if (!GET_USHORT(str_len)) {
				return false;
			}

			return GET_STRING(ret, str_len);
		}

		inline bool GET_STRING(std::string& ret, size_t str_len) {
			if (size() < str_len) {
				return false;
			}

			char* str = new char[str_len + 1];
			memcpy(str, p, str_len);
			str[str_len] = 0;
			ret.assign(str, str_len);
			delete[] str;
			p += str_len;
			return true;
		}

		inline bool GET_LSTRING(std::string& ret) {
			uint32_t str_len;
			if (!GET_ULONG(str_len)) {
				return false;
			}

			return GET_STRING(ret, str_len);
		}

		inline bool SKIP_N(uint32_t n) {
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
			buffer = (char*)calloc(1, 32 * sizeof(char));
			buffer_size = 32;
			size = 0;
		}

		~PropWriteStream() {
			free(buffer);
		}

		// non-copyable
		PropWriteStream(const PropWriteStream&) = delete;
		PropWriteStream& operator=(const PropWriteStream&) = delete;

		const char* getStream(uint32_t& _size) const {
			_size = size;
			return buffer;
		}

		//TODO: might need temp buffer and zero fill the memory chunk allocated by realloc
		template <typename T>
		inline void ADD_TYPE(T* add) {
			if (!reserve(sizeof(T))) {
				return;
			}

			memcpy(&buffer[size], (char*)add, sizeof(T));
			size += sizeof(T);
		}

		template <typename T>
		inline void ADD_VALUE(T add) {
			if (!reserve(sizeof(T))) {
				return;
			}

			memcpy(&buffer[size], &add, sizeof(T));
			size += sizeof(T);
		}

		inline void ADD_ULONG(uint32_t ret) {
			ADD_VALUE(ret);
		}

		inline void ADD_USHORT(uint16_t ret) {
			ADD_VALUE(ret);
		}

		inline void ADD_UCHAR(uint8_t ret) {
			ADD_VALUE(ret);
		}

		inline void ADD_STRING(const std::string& add) {
			size_t str_len = add.size();
			ADD_USHORT(str_len);
			ADD_STRING(add, str_len);
		}

		inline void ADD_LSTRING(const std::string& add) {
			size_t str_len = add.size();
			ADD_ULONG(str_len);
			ADD_STRING(add, str_len);
		}

		inline void ADD_STRING(const std::string& add, size_t str_len) {
			if (!reserve(str_len)) {
				return;
			}

			memcpy(&buffer[size], add.c_str(), str_len);
			size += str_len;
		}

	protected:
		inline bool reserve(size_t length) {
			if ((buffer_size - size) >= length) {
				return true;
			}

			buffer_size += ((length + 0x1F) & 0xFFFFFFE0);
			buffer = (char*)realloc(buffer, buffer_size);
			return buffer != nullptr;
		}

		char* buffer;
		uint32_t buffer_size;
		uint32_t size;
};

#endif
