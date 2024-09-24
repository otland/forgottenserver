// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_FILELOADER_H
#define FS_FILELOADER_H

namespace OTB {

using MappedFile = boost::iostreams::mapped_file_source;
using iterator = MappedFile::iterator;

struct Node
{
	std::vector<Node> children = {};
	iterator propsBegin, propsEnd;
	char type;

	static constexpr char ESCAPE = '\xFD';
	static constexpr char START = '\xFE';
	static constexpr char END = '\xFF';
};

class Loader
{
public:
	Loader(MappedFile file, Node root) : file{std::move(file)}, root{std::move(root)} {}

	const std::vector<Node>& children() const { return root.children; }
	auto begin() const { return root.propsBegin; }
	auto end() const { return root.propsEnd; }

private:
	MappedFile file;
	Node root;
};

Loader load(std::string_view filename, std::string_view acceptedIdentifier);

template <class T, class It>
T read(It& first, It const last)
{
	std::array<char, sizeof(T)> buf;
	auto it = buf.begin();

	while (it != buf.end() && first < last) {
		if (*first == Node::ESCAPE) {
			++first;
		}
		*it++ = *first++;
	}

	if (it != buf.end()) {
		throw std::invalid_argument("Not enough bytes to read.");
	}

	T out;
	std::memcpy(reinterpret_cast<char*>(&out), buf.data(), buf.size());
	return out;
}

template <class It>
std::string readString(It& first, It const last)
{
	auto len = read<uint16_t>(first, last);
	if (last - first < len) {
		throw std::invalid_argument("Not enough bytes to read as string.");
	}

	std::string out;
	out.reserve(len);

	auto start = first, end = first + len;
	while (start < end) {
		if (*start == Node::ESCAPE) {
			++start, ++end;
		}
		out.push_back(*start++);
	}

	first = end;
	return out;
}

template <class It>
void skip(It& first, It const last, const int len)
{
	if (last - first < len) {
		throw std::invalid_argument("Not enough bytes to skip.");
	}

	auto start = first, end = first + len;
	while (start < end) {
		if (*start == Node::ESCAPE) {
			++start, ++end;
		}
		++start;
	}

	first = end;
}

} // namespace OTB

class PropStream
{
public:
	void init(const char* a, size_t size)
	{
		p = a;
		end = a + size;
	}

	size_t size() const { return end - p; }

	template <typename T>
	bool read(T& ret)
	{
		if (size() < sizeof(T)) {
			return false;
		}

		memcpy(&ret, p, sizeof(T));
		p += sizeof(T);
		return true;
	}

	std::pair<std::string_view, bool> readString()
	{
		uint16_t strLen;
		if (!read<uint16_t>(strLen)) {
			return {"", false};
		}

		if (size() < strLen) {
			return {"", false};
		}

		std::string_view ret{p, strLen};
		p += strLen;
		return {ret, true};
	}

	bool skip(size_t n)
	{
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

	std::string_view getStream() const { return {buffer.data(), buffer.size()}; }

	void clear() { buffer.clear(); }

	template <typename T>
	void write(T add)
	{
		char* addr = reinterpret_cast<char*>(&add);
		std::copy(addr, addr + sizeof(T), std::back_inserter(buffer));
	}

	void writeString(std::string_view str)
	{
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

#endif // FS_FILELOADER_H
