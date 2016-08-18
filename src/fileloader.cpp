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

#include "otpch.h"

#include "fileloader.h"

FileLoader::~FileLoader()
{
	if (file) {
		fclose(file);
		file = nullptr;
	}

	NodeStruct::clearNet(root);
	delete[] buffer;

	for (auto& i : cached_data) {
		delete[] i.data;
	}
}

bool FileLoader::openFile(const char* filename, const char* accept_identifier)
{
	file = fopen(filename, "rb");
	if (!file) {
		lastError = ERROR_CAN_NOT_OPEN;
		return false;
	}

	char identifier[4];
	if (fread(identifier, 1, 4, file) < 4) {
		fclose(file);
		file = nullptr;
		lastError = ERROR_EOF;
		return false;
	}

	// The first four bytes must either match the accept identifier or be 0x00000000 (wildcard)
	if (memcmp(identifier, accept_identifier, 4) != 0 && memcmp(identifier, "\0\0\0\0", 4) != 0) {
		fclose(file);
		file = nullptr;
		lastError = ERROR_INVALID_FILE_VERSION;
		return false;
	}

	fseek(file, 0, SEEK_END);
	int32_t file_size = ftell(file);
	cache_size = std::min<uint32_t>(32768, std::max<uint32_t>(file_size / 20, 8192)) & ~0x1FFF;

	if (!safeSeek(4)) {
		lastError = ERROR_INVALID_FORMAT;
		return false;
	}

	delete root;
	root = new NodeStruct();
	root->start = 4;

	int32_t byte;
	if (safeSeek(4) && readByte(byte) && byte == NODE_START) {
		return parseNode(root);
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
							lastError = ERROR_INVALID_FORMAT;
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

	if (node->propsSize >= buffer_size) {
		delete[] buffer;

		while (node->propsSize >= buffer_size) {
			buffer_size *= 2;
		}

		buffer = new uint8_t[buffer_size];
	}

	//get buffer
	if (!readBytes(node->propsSize, node->start + 2)) {
		return nullptr;
	}

	//unscape buffer
	size_t j = 0;
	bool escaped = false;
	for (uint32_t i = 0; i < node->propsSize; ++i, ++j) {
		if (buffer[i] == ESCAPE_CHAR) {
			//escape char found, skip it and write next
			buffer[j] = buffer[++i];
			//is neede a displacement for next bytes
			escaped = true;
		} else if (escaped) {
			//perform that displacement
			buffer[j] = buffer[i];
		}
	}

	size = j;
	return buffer;
}

bool FileLoader::getProps(const NODE node, PropStream& props)
{
	size_t size;
	if (const uint8_t* a = getProps(node, size)) {
		props.init(reinterpret_cast<const char*>(a), size); // does not break strict aliasing
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

	type = root->type;
	return root;
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
	if (cache_index == NO_VALID_CACHE) {
		lastError = ERROR_CACHE_ERROR;
		return false;
	}

	if (cache_offset >= cached_data[cache_index].size) {
		int32_t pos = cache_offset + cached_data[cache_index].base;
		int32_t tmp = getCacheBlock(pos);
		if (tmp < 0) {
			return false;
		}

		cache_index = tmp;
		cache_offset = pos - cached_data[cache_index].base;
		if (cache_offset >= cached_data[cache_index].size) {
			return false;
		}
	}

	value = cached_data[cache_index].data[cache_offset++];
	return true;
}

inline bool FileLoader::readBytes(uint32_t size, int32_t pos)
{
	//seek at pos
	uint32_t remain = size;
	uint8_t* buf = this->buffer;
	do {
		//prepare cache
		uint32_t i = getCacheBlock(pos);
		if (i == NO_VALID_CACHE) {
			return false;
		}

		cache_index = i;
		cache_offset = pos - cached_data[i].base;

		//get maximum read block size and calculate remaining bytes
		uint32_t reading = std::min<int32_t>(remain, cached_data[i].size - cache_offset);
		remain -= reading;

		//read it
		memcpy(buf, cached_data[cache_index].data + cache_offset, reading);

		//update variables
		cache_offset += reading;
		buf += reading;
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

	cache_index = i;
	cache_offset = pos - cached_data[i].base;
	return true;
}

inline bool FileLoader::safeTell(int32_t& pos)
{
	if (cache_index == NO_VALID_CACHE) {
		lastError = ERROR_CACHE_ERROR;
		return false;
	}

	pos = cached_data[cache_index].base + cache_offset - 1;
	return true;
}

inline uint32_t FileLoader::getCacheBlock(uint32_t pos)
{
	bool found = false;
	uint32_t i, base_pos = pos & ~(cache_size - 1);

	for (i = 0; i < CACHE_BLOCKS; i++) {
		if (cached_data[i].loaded) {
			if (cached_data[i].base == base_pos) {
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
	int32_t i, loading_cache = -1, base_pos = pos & ~(cache_size - 1);

	for (i = 0; i < CACHE_BLOCKS; i++) {
		if (!cached_data[i].loaded) {
			loading_cache = i;
			break;
		}
	}

	if (loading_cache == -1) {
		for (i = 0; i < CACHE_BLOCKS; i++) {
			if (std::abs(static_cast<long>(cached_data[i].base) - base_pos) > static_cast<long>(2 * cache_size)) {
				loading_cache = i;
				break;
			}
		}

		if (loading_cache == -1) {
			loading_cache = 0;
		}
	}

	if (cached_data[loading_cache].data == nullptr) {
		cached_data[loading_cache].data = new uint8_t[cache_size];
	}

	cached_data[loading_cache].base = base_pos;

	if (fseek(file, cached_data[loading_cache].base, SEEK_SET) != 0) {
		lastError = ERROR_SEEK_ERROR;
		return -1;
	}

	uint32_t size = fread(cached_data[loading_cache].data, 1, cache_size, file);
	cached_data[loading_cache].size = size;

	if (size < (pos - cached_data[loading_cache].base)) {
		lastError = ERROR_SEEK_ERROR;
		return -1;
	}

	cached_data[loading_cache].loaded = 1;
	return loading_cache;
}
