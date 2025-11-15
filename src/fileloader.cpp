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

	for (; first != last; ++first) {
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

} // namespace OTB
