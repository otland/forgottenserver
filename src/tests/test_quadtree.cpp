#define BOOST_TEST_MODULE quadtree

#include "../otpch.h"

#include "../quadtree.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_find_in_range_single_creature)
{
	auto npc1 = new Npc();
	tfs::map::quadtree::push_creature(12, 12, npc1);

	int count = 0;
	for (auto creature : tfs::map::quadtree::find_in_range(10, 10, 20, 20)) {
		BOOST_TEST(creature == npc1);
		count++;
	}

	BOOST_TEST(count == 1);
}

BOOST_AUTO_TEST_CASE(test_find_in_range_multiple_creature)
{
	auto npc1 = new Npc();
	auto npc2 = new Npc();
	tfs::map::quadtree::push_creature(10, 10, npc1);
	tfs::map::quadtree::push_creature(15, 15, npc2);

	int count = 0;
	for (auto creature : tfs::map::quadtree::find_in_range(5, 5, 20, 20)) {
		BOOST_TEST(creature == creature1 || creature == creature2);
		count++;
	}

	BOOST_TEST(count == 2);
}

BOOST_AUTO_TEST_CASE(test_find_in_range_no_creatures)
{
	auto npc1 = new Npc();
	auto npc2 = new Npc();
	tfs::map::quadtree::push_creature(50, 50, npc1);
	tfs::map::quadtree::push_creature(60, 60, npc2);

	int count = 0;
	for (auto creature : tfs::map::quadtree::find_in_range(5, 5, 20, 20)) {
		BOOST_TEST(creature != npc1 && creature != npc2);
		count++;
	}

	BOOST_TEST(count == 0);
}
