// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "wildcardtree.h"

#include <stack>
#include <tuple>

namespace {

auto wildcardtree_root = new WildcardTreeNode(false);

const WildcardTreeNode* WildcardTreeNode::find_child(char c) const
{
	auto it = children.find(c);
	if (it == children.end()) {
		return nullptr;
	}
	return it->second;
}

WildcardTreeNode* WildcardTreeNode::find_child(char c)
{
	auto it = children.find(c);
	if (it == children.end()) {
		return nullptr;
	}
	return it->second;
}

WildcardTreeNode* WildcardTreeNode::add_child(char c, bool breakpoint)
{
	if (auto child = find_child(c)) {
		// If the child already exists, update its breakpoint if necessary
		if (breakpoint && !child->breakpoint) {
			child->breakpoint = true;
		}
		return child;
	}

	auto node = new WildcardTreeNode(breakpoint);
	auto pair = children.emplace(c, node);
	return node;
}

} // namespace

namespace tfs::game::wildcardtree {

void add(std::string_view s)
{
	auto node = wildcardtree_root;

	auto length = s.length();
	// Iterate through the string, adding nodes for each character except the last
	for (size_t pos = 0; pos < length - 1; ++pos) {
		// Add child nodes without marking breakpoints
		node = node->add_child(s[pos], false);
	}

	// Mark the final character as a breakpoint
	node->add_child(s.back(), true);
}

void remove(std::string_view s)
{
	auto node = wildcardtree_root;

	// Stack to keep track of the path as we traverse the tree
	std::stack<WildcardTreeNode*> path;
	path.push(node);

	auto length = s.length();
	// Traverse the tree based on the input string
	for (size_t pos = 0; pos < length; ++pos) {
		node = node->find_child(s[pos]);
		if (!node) {
			return;
		}
		path.push(node);
	}

	node->breakpoint = false;

	// Remove orphaned nodes
	while (true) {
		node = path.top();
		path.pop();
		if (path.empty()) {
			// Stop if the root is reached
			break;
		}

		if (node->breakpoint || !node->children.empty()) {
			// Stop if the node has children or is a breakpoint
			break;
		}

		node = path.top(); // Go back to the parent node

		auto it = node->children.find(s[--length]);
		if (it != node->children.end()) {
			// Erase the child node from the parent if it exists
			node->children.erase(it);
		}
	}
}

std::pair<WildcardTreeSearchResult, std::string> search(std::string_view query)
{
	auto node = wildcardtree_root;

	for (auto c : query) {
		node = node->find_child(c);
		if (!node) {
			return std::make_pair(WildcardTreeSearchResult::NotFound, "");
		}
	}

	std::string result(query);

	// Continue traversal until ambiguity or the end of the string is found
	while (true) {
		const auto size = node->children.size();
		if (size == 0) {
			// Exact match found
			return std::make_pair(WildcardTreeSearchResult::Found, result);
		}

		if (size > 1 || node->breakpoint) {
			// Return ambiguous if multiple child nodes exist or a breakpoint is reached
			return std::make_pair(WildcardTreeSearchResult::Ambiguous, "");
		}

		auto it = node->children.begin();
		result += it->first;
		node = it->second;
	}
}

} // namespace tfs::game::wildcardtree
