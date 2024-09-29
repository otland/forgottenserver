// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_WILDCARDTREE_H
#define FS_WILDCARDTREE_H

#include "enums.h"

/**
 * @class WildcardTreeNode
 * @brief Represents a node in a wildcard tree. Each node can store children nodes, with
 * an optional "breakpoint" to mark the end of a string.
 */
class WildcardTreeNode
{
public:
	/**
	 * @brief Constructs a WildcardTreeNode with an optional breakpoint.
	 * @param {breakpoint} A boolean
	 * value indicating if the node represents a breakpoint (i.e., the end of a string).
	 */
	explicit WildcardTreeNode(bool breakpoint) : breakpoint(breakpoint) {}

	/**
	 * @brief Move constructor for WildcardTreeNode.
	 * @param other The node to move from.
	 */
	WildcardTreeNode(WildcardTreeNode&& other) = default;

	/**
	 * @enum SearchResult
	 * @brief Represents the result of a search operation in the wildcard tree.
	 *
	 * - Found: The exact string or path was found.
	 * - NotFound: The string or path does not exist in the tree.
	 * - Ambiguous: The search result is ambiguous (e.g., multiple valid continuations exist).
	 */
	enum SearchResult
	{
		Found,
		NotFound,
		Ambiguous,
	};

	/// Deleted copy constructor to ensure WildcardTreeNode is non-copyable.
	WildcardTreeNode(const WildcardTreeNode&) = delete;
	/// Deleted assignment operator to ensure WildcardTreeNode is non-copyable.
	WildcardTreeNode& operator=(const WildcardTreeNode&) = delete;

	/**
	 * @brief Adds a string to the wildcard tree.
	 *
	 * This method breaks the string into characters
	 * and adds them as children nodes.
	 * If the string already exists, it updates the final node to represent a
	 * breakpoint.
	 *
	 * @param {s} The string to be added.
	 */
	void add(const std::string& s);

	/**
	 * @brief Removes a string from the wildcard tree.
	 *
	 * This method traverses the tree based on
	 * the input string and marks the final node as non-breakpoint.
	 * If the node becomes orphaned (no children and
	 * not a breakpoint), it is removed.
	 *
	 * @param {s} The string to be removed.
	 */
	void remove(const std::string& s);

	/**
	 * @brief Searches for a string in the wildcard tree.
	 *
	 * This method traverses the tree based on
	 * the input query string and returns the search result and matched string.
	 *
	 * @param {query} The string
	 * to search for.
	 *
	 * @return A pair containing the search result and the matched string.
	 */
	std::pair<WildcardTreeNode::SearchResult, std::string> search(const std::string& query) const;

private:
	/**
	 * @brief Finds a child node corresponding to a character.
	 * @param {c} The character to search for
	 * among the children.
	 * @return A pointer to the child node if found, otherwise nullptr.
	 */
	WildcardTreeNode* find_child(char c);

	/**
	 * @brief Finds a child node corresponding to a character (const version).
	 * @param {c} The character
	 * to search for among the children.
	 * @return A const pointer to the child node if found, otherwise nullptr.
	 */
	const WildcardTreeNode* find_child(char c) const;

	/**
	 * @brief Adds a child node for a specific character.
	 *
	 * If the child node for the character
	 * already exists, it will be updated.
	 * If it does not exist, a new node will be created.
	 *
	 *
	 * @param {c} The character to add as a child node.
	 * @param {breakpoint} A boolean value indicating if this
	 * node represents a breakpoint.
	 * @return A pointer to the added or existing child node.
	 */
	WildcardTreeNode* add_child(char c, bool breakpoint);

	/// A map of child nodes indexed by characters.
	std::map<char, WildcardTreeNode> children;

	/// Indicates whether this node represents a breakpoint (end of a string).
	bool breakpoint;
};

#endif // FS_WILDCARDTREE_H
