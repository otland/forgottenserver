/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2014  Mark Samman <mark.samman@gmail.com>
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

#include "otpch.h"

#include "fileloader.h"

FileLoader::FileLoader() : m_cached_data()
{
	m_file = nullptr;
	m_root = nullptr;
	m_buffer = new uint8_t[1024];
	m_buffer_size = 1024;
	m_lastError = ERROR_NONE;

	//cache
	m_cache_size = 0;
	m_cache_index = NO_VALID_CACHE;
	m_cache_offset = NO_VALID_CACHE;
}

FileLoader::~FileLoader()
{
	if (m_file) {
		fclose(m_file);
		m_file = nullptr;
	}

	NodeStruct::clearNet(m_root);
	delete[] m_buffer;

	for (int32_t i = 0; i < CACHE_BLOCKS; i++) {
		delete[] m_cached_data[i].data;
	}
}

bool FileLoader::openFile(const char* filename, const char* accept_identifier)
{
	m_file = fopen(filename, "rb");
	if (!m_file) {
		m_lastError = ERROR_CAN_NOT_OPEN;
		return false;
	}

	char identifier[4];
	if (fread(identifier, 1, 4, m_file) < 4) {
		fclose(m_file);
		m_file = nullptr;
		m_lastError = ERROR_EOF;
		return false;
	}

	// The first four bytes must either match the accept identifier or be 0x00000000 (wildcard)
	if (memcmp(identifier, accept_identifier, 4) != 0 && memcmp(identifier, "\0\0\0\0", 4) != 0) {
		fclose(m_file);
		m_file = nullptr;
		m_lastError = ERROR_INVALID_FILE_VERSION;
		return false;
	}

	fseek(m_file, 0, SEEK_END);
	int32_t file_size = ftell(m_file);
	m_cache_size = std::min<uint32_t>(32768, std::max<uint32_t>(file_size / 20, 8192)) & ~0x1FFF;

	if (!safeSeek(4)) {
		m_lastError = ERROR_INVALID_FORMAT;
		return false;
	}

	delete m_root;
	m_root = new NodeStruct();
	m_root->start = 4;

	int32_t byte;
	if (safeSeek(4) && readByte(byte) && byte == NODE_START) {
		return parseNode(m_root);
	}

	return false;
}

bool FileLoader::parseNode(NODE node)
{
	int32_t byte, pos;
	NODE currentNode = node;

	while (readByte(byte)) {
		currentNode->type = byte;
		bool setPropsSize = false;

		while (true) {
			if (!readByte(byte)) {
				return false;
			}

			bool skipNode = false;

			switch (byte) {
				case NODE_START: {
					//child node start
					if (!safeTell(pos)) {
						return false;
					}

					NODE childNode = new NodeStruct();
					childNode->start = pos;
					currentNode->propsSize = pos - currentNode->start - 2;
					currentNode->child = childNode;

					setPropsSize = true;

					if (!parseNode(childNode)) {
						return false;
					}

					break;
				}

				case NODE_END: {
					//current node end
					if (!setPropsSize) {
						if (!safeTell(pos)) {
							return false;
						}

						currentNode->propsSize = pos - currentNode->start - 2;
					}

					if (!readByte(byte)) {
						return true;
					}

					switch (byte) {
						case NODE_START: {
							//starts next node
							if (!safeTell(pos)) {
								return false;
							}

							skipNode = true;
							NODE nextNode = new NodeStruct();
							nextNode->start = pos;
							currentNode->next = nextNode;
							currentNode = nextNode;
							break;
						}

						case NODE_END:
							return safeTell(pos) && safeSeek(pos);

						default:
							m_lastError = ERROR_INVALID_FORMAT;
							return false;
					}

					break;
				}

				case ESCAPE_CHAR: {
					if (!readByte(byte)) {
						return false;
					}

					break;
				}

				default:
					break;
			}

			if (skipNode) {
				break;
			}
		}
	}
	return false;
}

const uint8_t* FileLoader::getProps(const NODE node, size_t& size)
{
	if (!node) {
		return nullptr;
	}

	if (node->propsSize >= m_buffer_size) {
		delete[] m_buffer;

		while (node->propsSize >= m_buffer_size) {
			m_buffer_size *= 2;
		}

		m_buffer = new uint8_t[m_buffer_size];
	}

	//get buffer
	if (!readBytes(m_buffer, node->propsSize, node->start + 2)) {
		return nullptr;
	}

	//unscape buffer
	size_t j = 0;
	bool escaped = false;
	for (uint32_t i = 0; i < node->propsSize; ++i, ++j) {
		if (m_buffer[i] == ESCAPE_CHAR) {
			//escape char found, skip it and write next
			m_buffer[j] = m_buffer[++i];
			//is neede a displacement for next bytes
			escaped = true;
		} else if (escaped) {
			//perform that displacement
			m_buffer[j] = m_buffer[i];
		}
	}

	size = j;
	return m_buffer;
}

bool FileLoader::getProps(const NODE node, PropStream& props)
{
	size_t size;
	if (const uint8_t* a = getProps(node, size)) {
		props.init((char*)a, size);
		return true;
	}

	props.init(nullptr, 0);
	return false;
}

NODE FileLoader::getChildNode(const NODE parent, uint32_t& type)
{
	if (parent) {
		NODE child = parent->child;
		if (child) {
			type = child->type;
		}

		return child;
	}

	type = m_root->type;
	return m_root;
}

NODE FileLoader::getNextNode(const NODE prev, uint32_t& type)
{
	if (!prev) {
		return NO_NODE;
	}

	NODE next = prev->next;
	if (next) {
		type = next->type;
	}
	return next;
}

inline bool FileLoader::readByte(int32_t& value)
{
	if (m_cache_index == NO_VALID_CACHE) {
		m_lastError = ERROR_CACHE_ERROR;
		return false;
	}

	if (m_cache_offset >= m_cached_data[m_cache_index].size) {
		int32_t pos = m_cache_offset + m_cached_data[m_cache_index].base;
		int32_t tmp = getCacheBlock(pos);
		if (tmp < 0) {
			return false;
		}

		m_cache_index = tmp;
		m_cache_offset = pos - m_cached_data[m_cache_index].base;
		if (m_cache_offset >= m_cached_data[m_cache_index].size) {
			return false;
		}
	}

	value = m_cached_data[m_cache_index].data[m_cache_offset++];
	return true;
}

inline bool FileLoader::readBytes(uint8_t* buffer, uint32_t size, int32_t pos)
{
	//seek at pos
	uint32_t reading, remain = size, bufferPos = 0;
	do {
		//prepare cache
		uint32_t i = getCacheBlock(pos);
		if (i == NO_VALID_CACHE) {
			return false;
		}

		m_cache_index = i;
		m_cache_offset = pos - m_cached_data[i].base;

		//get maximum read block size and calculate remaining bytes
		reading = std::min<int32_t>(remain, m_cached_data[i].size - m_cache_offset);
		remain -= reading;

		//read it
		memcpy(buffer + bufferPos, m_cached_data[m_cache_index].data + m_cache_offset, reading);

		//update variables
		m_cache_offset += reading;
		bufferPos += reading;
		pos += reading;
	} while (remain > 0);
	return true;
}

inline bool FileLoader::safeSeek(uint32_t pos)
{
	uint32_t i = getCacheBlock(pos);
	if (i == NO_VALID_CACHE) {
		return false;
	}

	m_cache_index = i;
	m_cache_offset = pos - m_cached_data[i].base;
	return true;
}

inline bool FileLoader::safeTell(int32_t& pos)
{
	if (m_cache_index == NO_VALID_CACHE) {
		m_lastError = ERROR_CACHE_ERROR;
		return false;
	}

	pos = m_cached_data[m_cache_index].base + m_cache_offset - 1;
	return true;
}

inline uint32_t FileLoader::getCacheBlock(uint32_t pos)
{
	bool found = false;
	uint32_t i, base_pos = pos & ~(m_cache_size - 1);

	for (i = 0; i < CACHE_BLOCKS; i++) {
		if (m_cached_data[i].loaded) {
			if (m_cached_data[i].base == base_pos) {
				found = true;
				break;
			}
		}
	}

	if (!found) {
		i = loadCacheBlock(pos);
	}

	return i;
}

int32_t FileLoader::loadCacheBlock(uint32_t pos)
{
	int32_t i, loading_cache = -1, base_pos = pos & ~(m_cache_size - 1);

	for (i = 0; i < CACHE_BLOCKS; i++) {
		if (!m_cached_data[i].loaded) {
			loading_cache = i;
			break;
		}
	}

	if (loading_cache == -1) {
		for (i = 0; i < CACHE_BLOCKS; i++) {
			if (abs((long)m_cached_data[i].base - base_pos) > (long)(2 * m_cache_size)) {
				loading_cache = i;
				break;
			}
		}

		if (loading_cache == -1) {
			loading_cache = 0;
		}
	}

	if (m_cached_data[loading_cache].data == nullptr) {
		m_cached_data[loading_cache].data = new uint8_t[m_cache_size];
	}

	m_cached_data[loading_cache].base = base_pos;

	if (fseek(m_file, m_cached_data[loading_cache].base, SEEK_SET) != 0) {
		m_lastError = ERROR_SEEK_ERROR;
		return -1;
	}

	uint32_t size = fread(m_cached_data[loading_cache].data, 1, m_cache_size, m_file);
	m_cached_data[loading_cache].size = size;

	if (size < (pos - m_cached_data[loading_cache].base)) {
		m_lastError = ERROR_SEEK_ERROR;
		return -1;
	}

	m_cached_data[loading_cache].loaded = 1;
	return loading_cache;
}
