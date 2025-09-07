// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "fileloader.h"

#include <stack>

namespace OTB {

constexpr Identifier wildcard = {{'\0', '\0', '\0', '\0'}};

Loader::Loader(const std::string& fileName, const Identifier& acceptedIdentifier)
{
	try {
		fileContents.open(fileName); // may throw if file does not exists
	} catch (...) {
		throw OTBMException(OPEN_FAILED);
	}

	constexpr auto minimalSize = sizeof(Identifier) + sizeof(Node::START) + sizeof(Node::type) + sizeof(Node::END);
	if (fileContents.size() <= minimalSize) {
		throw OTBMException(INVALID_FILE_FORMAT);
	}

	Identifier fileIdentifier;
	std::copy(fileContents.begin(), fileContents.begin() + fileIdentifier.size(), fileIdentifier.begin());
	if (fileIdentifier != acceptedIdentifier && fileIdentifier != wildcard) {
		throw OTBMException(INVALID_FILE_FORMAT);
	}
}

using NodeStack = std::stack<Node*, std::vector<Node*>>;
static Node& getCurrentNode(const NodeStack& nodeStack)
{
	if (nodeStack.empty()) {
		throw OTBMException(INVALID_FILE_FORMAT);
	}
	return *nodeStack.top();
}

const Node& Loader::parseTree()
{
	auto it = fileContents.begin() + sizeof(Identifier);
	if (static_cast<uint8_t>(*it) != Node::START) {
		throw OTBMException(INVALID_FILE_FORMAT);
	}
	root.type = *(++it);
	root.propsBegin = ++it;
	NodeStack parseStack;
	parseStack.push(&root);

	for (; it != fileContents.end(); ++it) {
		switch (static_cast<uint8_t>(*it)) {
			case Node::START: {
				auto& currentNode = getCurrentNode(parseStack);
				if (currentNode.children.empty()) {
					currentNode.propsEnd = it;
				}
				currentNode.children.emplace_back();
				auto& child = currentNode.children.back();
				if (++it == fileContents.end()) {
					throw OTBMException(INVALID_FILE_FORMAT);
				}
				child.type = *it;
				child.propsBegin = it + sizeof(Node::type);
				parseStack.push(&child);
				break;
			}
			case Node::END: {
				auto& currentNode = getCurrentNode(parseStack);
				if (currentNode.children.empty()) {
					currentNode.propsEnd = it;
				}
				parseStack.pop();
				break;
			}
			case Node::ESCAPE: {
				if (++it == fileContents.end()) {
					throw OTBMException(INVALID_FILE_FORMAT);
				}
				break;
			}
			default: {
				break;
			}
		}
	}
	if (!parseStack.empty()) {
		throw OTBMException(INVALID_FILE_FORMAT);
	}

	return root;
}

bool Loader::getProps(const Node& node, PropStream& props)
{
	auto size = std::distance(node.propsBegin, node.propsEnd);
	if (size == 0) {
		return false;
	}
	propBuffer.resize(size);
	bool lastEscaped = false;

	auto escapedPropEnd =
	    std::copy_if(node.propsBegin, node.propsEnd, propBuffer.begin(), [&lastEscaped](const char& byte) {
		    lastEscaped = byte == static_cast<char>(Node::ESCAPE) && !lastEscaped;
		    return !lastEscaped;
	    });
	props.init(&propBuffer[0], std::distance(propBuffer.begin(), escapedPropEnd));
	return true;
}

} // namespace OTB
