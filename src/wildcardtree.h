// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_WILDCARDTREE_H
#define FS_WILDCARDTREE_H

#include "enums.h"

class WildcardTreeNode
{
public:
	enum SearchResult
	{
		Found,
		NotFound,
		Ambiguous,
	};

	explicit WildcardTreeNode(bool breakpoint) : breakpoint(breakpoint) {}
	WildcardTreeNode(WildcardTreeNode&& other) = default;

	// non-copyable
	WildcardTreeNode(const WildcardTreeNode&) = delete;
	WildcardTreeNode& operator=(const WildcardTreeNode&) = delete;

	void add(const std::string& str);
	void remove(const std::string& str);

	std::pair<WildcardTreeNode::SearchResult, std::string> search(const std::string& query) const;

private:
	WildcardTreeNode* find_child(char c);
	const WildcardTreeNode* find_child(char c) const;
	WildcardTreeNode* add_child(char c, bool breakpoint);

	std::map<char, WildcardTreeNode> children;
	bool breakpoint;
};

#endif // FS_WILDCARDTREE_H
