// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "fileloader.h"

namespace OTB {

namespace {

const auto wildcard = std::string_view{"\0\0\0\0", 4};

auto checkIdentifier(iterator it, std::string_view acceptedIdentifier)
{
	auto identifier = std::string_view(it, 4);
	return identifier == acceptedIdentifier || identifier == wildcard;
}

Node parseTree(iterator& first, const iterator last)
{
	Node node{.propsBegin = first + 1, .propsEnd = last, .type = *first};

	while (first != last) {
		switch (*first) {
			case Node::START: {
				if (node.children.empty()) {
					node.propsEnd = first;
				}

				if (++first == last) [[unlikely]] {
					throw std::invalid_argument("File overflow on start node.");
				}

				node.children.emplace_back(parseTree(first, last));
				break;
			}
			case Node::END: {
				if (node.children.empty()) {
					node.propsEnd = first;
				}

				return node;
			}
			case Node::ESCAPE: {
				if (++first == last) [[unlikely]] {
					throw std::invalid_argument("File overflow on escape node.");
				}
				break;
			}
		}
		++first;
	}

	throw std::invalid_argument("File underflow.");
}

} // namespace

Loader load(std::string_view filename, std::string_view acceptedIdentifier)
{
	MappedFile file{filename.data()};

	if (!checkIdentifier(file.begin(), acceptedIdentifier)) {
		throw std::invalid_argument("Invalid magic header.");
	}

	auto first = file.begin() + 4;
	if (*first != Node::START) {
		throw std::invalid_argument("Invalid first byte.");
	}

	return {std::move(file), parseTree(++first, file.end())};
}

[[nodiscard]] std::string readBytes(OTB::iterator& first, const OTB::iterator last, const size_t len)
{
	std::string out;
	out.reserve(len);

	auto end = first + len;
	while (first < end) {
		if (*first == Node::ESCAPE) [[unlikely]] {
			++first, ++end;
		}

		if (first == last) [[unlikely]] {
			throw std::invalid_argument("Not enough bytes to read.");
		}

		out.push_back(*first++);
	}

	if (first > last) [[unlikely]] {
		throw std::invalid_argument("Not enough bytes to read.");
	}

	return out;
}

[[nodiscard]] std::string readString(OTB::iterator& first, const OTB::iterator last)
{
	const auto len = read<uint16_t>(first, last);

	return readBytes(first, last, len);
}

void skip(OTB::iterator& first, const OTB::iterator last, const size_t len)
{
	auto end = first + len;
	while (first < end) {
		if (*first == Node::ESCAPE) [[unlikely]] {
			++first;
			++end;
		}
		++first;
	}

	if (first > last) [[unlikely]] {
		throw std::invalid_argument("Not enough bytes to skip.");
	}
}

} // namespace OTB
