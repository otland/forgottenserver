#define BOOST_TEST_MODULE wildcard_tree

#include "../otpch.h"

#include "../wildcardtree.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_wildcard_tree_single_words)
{
	tfs::game::wildcardtree::add("test");

    BOOST_CHECK_EQUAL(tfs::game::wildcardtree::search("tes").first, WildcardTreeNode::Found);
    BOOST_CHECK_EQUAL(tfs::game::wildcardtree::search("tes").second, "test");

    BOOST_CHECK_EQUAL(tfs::game::wildcardtree::search("test").first, WildcardTreeNode::Found);
    BOOST_CHECK_EQUAL(tfs::game::wildcardtree::search("test").second, "test");

    BOOST_CHECK_EQUAL(tfs::game::wildcardtree::search("testing").first, WildcardTreeNode::NotFound);
    BOOST_CHECK_EQUAL(tfs::game::wildcardtree::search("testing").second, "");
}

BOOST_AUTO_TEST_CASE(test_wildcard_tree_ambiguity)
{
	tfs::game::wildcardtree::add("test");
    tfs::game::wildcardtree::add("te");

    BOOST_CHECK_EQUAL(tfs::game::wildcardtree::search("te").first, WildcardTreeNode::Ambiguous);
    BOOST_CHECK_EQUAL(tfs::game::wildcardtree::search("te").second, "");

    BOOST_CHECK_EQUAL(tfs::game::wildcardtree::search("test").first, WildcardTreeNode::Found);
    BOOST_CHECK_EQUAL(tfs::game::wildcardtree::search("test").second, "test");
}

BOOST_AUTO_TEST_CASE(test_wildcard_tree_remove)
{
    tfs::game::wildcardtree::add("test");

    BOOST_CHECK_EQUAL(tfs::game::wildcardtree::search("test").first, WildcardTreeNode::Found);
    BOOST_CHECK_EQUAL(tfs::game::wildcardtree::search("test").second, "test");

    tfs::game::wildcardtree::remove("test");

    BOOST_CHECK_EQUAL(tfs::game::wildcardtree::search("test").first, WildcardTreeNode::NotFound);
}

BOOST_AUTO_TEST_CASE(test_wildcard_tree_partial_search)
{
    tfs::game::wildcardtree::add("test");
    tfs::game::wildcardtree::add("te");

    BOOST_CHECK_EQUAL(tfs::game::wildcardtree::search("te").first, WildcardTreeNode::Ambiguous);
    BOOST_CHECK_EQUAL(tfs::game::wildcardtree::search("te").second, "");

    BOOST_CHECK_EQUAL(tfs::game::wildcardtree::search("test").first, WildcardTreeNode::Found);
    BOOST_CHECK_EQUAL(tfs::game::wildcardtree::search("test").second, "test");

    tfs::game::wildcardtree::remove("test");

    BOOST_CHECK_EQUAL(tfs::game::wildcardtree::search("te").first, WildcardTreeNode::Found);
    BOOST_CHECK_EQUAL(tfs::game::wildcardtree::search("te").second, "te");

    BOOST_CHECK_EQUAL(tfs::game::wildcardtree::search("test").first, WildcardTreeNode::NotFound);
    BOOST_CHECK_EQUAL(tfs::game::wildcardtree::search("test").second, "");
}

BOOST_AUTO_TEST_CASE(test_wildcard_tree_search_after_remove)
{
    tfs::game::wildcardtree::add("test");
    tfs::game::wildcardtree::add("te");
    tfs::game::wildcardtree::remove("test");

    BOOST_CHECK_EQUAL(tfs::game::wildcardtree::search("test").first, WildcardTreeNode::NotFound);
    BOOST_CHECK_EQUAL(tfs::game::wildcardtree::search("test").second, "");

    BOOST_CHECK_EQUAL(tfs::game::wildcardtree::search("te").first, WildcardTreeNode::Found);
    BOOST_CHECK_EQUAL(tfs::game::wildcardtree::search("te").second, "te");
}
