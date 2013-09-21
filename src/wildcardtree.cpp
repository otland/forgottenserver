/**
 * The Forgotten Server - a server application for the MMORPG Tibia
 * Copyright (C) 2013  Mark Samman <mark.samman@gmail.com>
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

#include <stack>

#include "wildcardtree.h"

WildcardTreeNode* WildcardTreeNode::getChild(char ch)
{
	auto it = children.find(ch);
	if (it == children.end()) {
		return NULL;
	}
	return &it->second;
}

const WildcardTreeNode* WildcardTreeNode::getChild(char ch) const
{
	auto it = children.find(ch);
	if (it == children.end()) {
		return NULL;
	}
	return &it->second;
}

WildcardTreeNode* WildcardTreeNode::addChild(char ch, bool breakpoint)
{
	WildcardTreeNode* child = getChild(ch);
	if (child) {
		if (breakpoint && !child->breakpoint) {
			child->breakpoint = true;
		}
	} else {
		children[ch] = WildcardTreeNode(breakpoint); // TODO: Use emplace
	}
	return child;
}

void WildcardTreeNode::insert(const std::string& str)
{
	WildcardTreeNode* cur = this;

	size_t length = str.length() - 1;
	for (size_t pos = 0; pos < length; ++pos) {
		cur = cur->addChild(str[pos], false);
	}

	cur->addChild(str[length], true);
}

void WildcardTreeNode::remove(const std::string& str)
{
	WildcardTreeNode* cur = this;

	std::stack<WildcardTreeNode*> path;
	path.push(cur);
	size_t len = str.length();
	for (size_t pos = 0; pos < len; ++pos) {
		cur = cur->getChild(str[pos]);
		path.push(cur);
	}

	cur->breakpoint = false;

	do {
		cur = path.top();
		path.pop();

		if (!cur->children.empty() || cur->breakpoint || path.empty()) {
			break;
		}

		cur = path.top();

		auto it = cur->children.find(str[--len]);
		if (it != cur->children.end()) {
			cur->children.erase(it);
		}
	} while (true);
}

ReturnValue WildcardTreeNode::findOne(const std::string& query, std::string& result) const
{
	const WildcardTreeNode* cur = this;
	for (size_t pos = 0; pos < query.length(); ++pos) {
		cur = cur->getChild(query[pos]);
		if (!cur) {
			return RET_PLAYERWITHTHISNAMEISNOTONLINE;
		}
	}

	result = query;

	do {
		size_t size = cur->children.size();
		if (size == 0) {
			return RET_NOERROR;
		} else if (size > 1 || cur->breakpoint) {
			return RET_NAMEISTOOAMBIGIOUS;
		}

		auto it = cur->children.begin();
		result += it->first;
		cur = &it->second;
	} while (true);
}
