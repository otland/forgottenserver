#define BOOST_TEST_MODULE wildcard_tree

#include "../otpch.h"

#include "../wildcardtree.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_wildcard_tree_single_words)
{
	WildcardTreeNode root(false);

	root.insert("test");

    BOOST_CHECK_EQUAL(root.search("tes").first, WildcardTreeNode::NotFound);
    BOOST_CHECK_EQUAL(root.search("test").first, WildcardTreeNode::Found);
    BOOST_CHECK_EQUAL(root.search("testing").first, WildcardTreeNode::NotFound);
}

BOOST_AUTO_TEST_CASE(test_wildcard_tree_ambiguity)
{
	WildcardTreeNode root(false);

	root.insert("test");
    root.insert("te");

    BOOST_CHECK_EQUAL(root.search("te").first, WildcardTreeNode::Ambiguous);
    BOOST_CHECK_EQUAL(root.search("test").first, WildcardTreeNode::Found);
}

BOOST_AUTO_TEST_CASE(test_wildcard_tree_remove)
{
    WildcardTreeNode root(false);

    root.insert("test");
    BOOST_CHECK_EQUAL(root.search("test").first, WildcardTreeNode::Found);

    root.remove("test");
    BOOST_CHECK_EQUAL(root.search("test").first, WildcardTreeNode::NotFound);
}

BOOST_AUTO_TEST_CASE(test_wildcard_tree_partial_search)
{
    WildcardTreeNode root(false);
    root.insert("test");
    root.insert("te");

    BOOST_CHECK_EQUAL(root.search("te").first, WildcardTreeNode::Ambiguous);
    BOOST_CHECK_EQUAL(root.search("test").first, WildcardTreeNode::Found);

    root.remove("test");
    BOOST_CHECK_EQUAL(root.search("te").first, WildcardTreeNode::Found);
    BOOST_CHECK_EQUAL(root.search("test").first, WildcardTreeNode::NotFound);
}
