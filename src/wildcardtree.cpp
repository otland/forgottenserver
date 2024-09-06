// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "wildcardtree.h"

#include <stack>
#include <tuple>

WildcardTreeNode* WildcardTreeNode::getChild(char ch)
{
	auto it = children.find(ch);
	if (it == children.end()) {
		return nullptr;
	}
	return &it->second;
}

const WildcardTreeNode* WildcardTreeNode::getChild(char ch) const
{
	auto it = children.find(ch);
	if (it == children.end()) {
		return nullptr;
	}
	return &it->second;
}

WildcardTreeNode* WildcardTreeNode::addChild(char ch, bool breakpoint)
{
	if (auto child = getChild(ch)) {
		if (breakpoint && !child->breakpoint) {
			child->breakpoint = true;
		}
		return child;
	}

	auto pair =
	    children.emplace(std::piecewise_construct, std::forward_as_tuple(ch), std::forward_as_tuple(breakpoint));
	return &pair.first->second;
}

void WildcardTreeNode::insert(const std::string& str)
{
	auto node = this;

	auto length = str.length() - 1;
	for (size_t pos = 0; pos < length; ++pos) {
		node = node->addChild(str[pos], false);
	}
	node->addChild(str[length], true);
}

void WildcardTreeNode::remove(const std::string& str)
{
	auto node = this;

	std::stack<WildcardTreeNode*> path;
	path.push(node);

	auto len = str.length();
	for (size_t pos = 0; pos < len; ++pos) {
		node = node->getChild(str[pos]);
		if (!node) {
			return;
		}
		path.push(node);
	}

	node->breakpoint = false;

	do {
		node = path.top();
		path.pop();

		if (!node->children.empty() || node->breakpoint || path.empty()) {
			break;
		}

		node = path.top();

		auto it = node->children.find(str[--len]);
		if (it != node->children.end()) {
			node->children.erase(it);
		}
	} while (true);
}

std::pair<WildcardTreeNode::SearchResult, std::string> WildcardTreeNode::search(const std::string& query) const
{
	auto node = this;

	for (auto c : query) {
		node = node->getChild(c);
		if (!node) {
			return std::make_pair(NotFound, "");
		}
	}

	auto result = query;

	do {
		auto size = node->children.size();
		if (size == 0) {
			return std::make_pair(Found, result);
		} else if (size > 1 || node->breakpoint) {
			return std::make_pair(Ambiguous, "");
		}

		auto it = node->children.begin();
		result += it->first;
		node = &it->second;
	} while (true);
}
