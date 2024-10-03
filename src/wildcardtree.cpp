// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "wildcardtree.h"

#include <stack>
#include <tuple>

void WildcardTreeNode::add(const std::string& s)
{
	auto node = shared_from_this();

	auto length = s.length() - 1;
	for (size_t pos = 0; pos < length; ++pos) {
		node = node->add_child(s[pos], false);
	}
	node->add_child(s[length], true);
}

void WildcardTreeNode::remove(const std::string& s)
{
	auto node = shared_from_this();

	std::stack<std::shared_ptr<WildcardTreeNode>> path;
	path.push(node);

	auto len = s.length();
	for (size_t pos = 0; pos < len; ++pos) {
		node = node->find_child(s[pos]);
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

		auto it = node->children.find(s[--len]);
		if (it != node->children.end()) {
			node->children.erase(it);
		}
	} while (true);
}

std::pair<WildcardTreeNode::SearchResult, std::string> WildcardTreeNode::search(const std::string& query) const
{
	auto node = shared_from_this();

	for (auto c : query) {
		node = node->find_child(c);
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
		node = it->second;
	} while (true);
}

std::shared_ptr<WildcardTreeNode> WildcardTreeNode::find_child(char c)
{
	auto it = children.find(c);
	if (it == children.end()) {
		return nullptr;
	}
	return it->second;
}

std::shared_ptr<const WildcardTreeNode> WildcardTreeNode::find_child(char c) const
{
	auto it = children.find(c);
	if (it == children.end()) {
		return nullptr;
	}
	return it->second;
}

std::shared_ptr<WildcardTreeNode> WildcardTreeNode::add_child(char c, bool breakpoint)
{
	if (auto child = find_child(c)) {
		if (breakpoint && !child->breakpoint) {
			child->breakpoint = true;
		}
		return child;
	}

	auto pair = children.emplace(c, std::make_shared<WildcardTreeNode>(breakpoint));
	return pair.first->second;
}
