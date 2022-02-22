// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_WILDCARDTREE_H_054C9BA46A1D4EA4B7C77ECE60ED4DEB
#define FS_WILDCARDTREE_H_054C9BA46A1D4EA4B7C77ECE60ED4DEB

#include "enums.h"

class WildcardTreeNode
{
	public:
		explicit WildcardTreeNode(bool breakpoint) : breakpoint(breakpoint) {}
		WildcardTreeNode(WildcardTreeNode&& other) = default;

		// non-copyable
		WildcardTreeNode(const WildcardTreeNode&) = delete;
		WildcardTreeNode& operator=(const WildcardTreeNode&) = delete;

		WildcardTreeNode* getChild(char ch);
		const WildcardTreeNode* getChild(char ch) const;
		WildcardTreeNode* addChild(char ch, bool breakpoint);

		void insert(const std::string& str);
		void remove(const std::string& str);

		ReturnValue findOne(const std::string& query, std::string& result) const;

	private:
		std::map<char, WildcardTreeNode> children;
		bool breakpoint;
};

#endif
