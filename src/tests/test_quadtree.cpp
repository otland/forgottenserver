#define BOOST_TEST_MODULE quadtree

#include "../otpch.h"

#include "../npc.h"
#include "../quadtree.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_find_in_range_single_creature)
{
	auto npc1 = new Npc("Alchemist");
	tfs::map::quadtree::push_creature(5, 5, npc1);

	auto creatures = 0;
	for (auto creature : tfs::map::quadtree::find_in_range(0, 0, 10, 10)) {
		BOOST_CHECK(creature == npc1);
		creatures++;
	}

	BOOST_TEST(creatures == 1);
}

BOOST_AUTO_TEST_CASE(test_find_in_range_multiple_creature)
{
	auto npc1 = new Npc("Guard");
	tfs::map::quadtree::push_creature(5, 5, npc1);

	auto npc2 = new Npc("Merchant");
	tfs::map::quadtree::push_creature(10, 10, npc2);

	auto npc3 = new Npc("Healer");
	tfs::map::quadtree::push_creature(5, 10, npc3);

	auto npc4 = new Npc("Quest Giver");
	tfs::map::quadtree::push_creature(10, 5, npc4);

	auto creatures = 0;
	for (auto creature : tfs::map::quadtree::find_in_range(0, 0, 15, 15)) {
		BOOST_CHECK(creature == npc1 || creature == npc2 || creature == npc3 || creature == npc4);
		creatures++;
	}

	BOOST_TEST(creatures == 4);
}

BOOST_AUTO_TEST_CASE(test_find_in_range_no_creatures)
{
	auto npc1 = new Npc("Herbalist");
	tfs::map::quadtree::push_creature(10, 10, npc1);

	BOOST_TEST(tfs::map::quadtree::find_in_range(0, 0, 5, 5).empty());

	auto npc2 = new Npc("Apprentice");
	tfs::map::quadtree::push_creature(15, 15, npc2);

	BOOST_TEST(tfs::map::quadtree::find_in_range(0, 0, 5, 5).empty());
}

BOOST_AUTO_TEST_CASE(test_move_single_creature)
{
	auto npc1 = new Npc("Stablemaster");
	tfs::map::quadtree::push_creature(5, 5, npc1);

	BOOST_TEST(!tfs::map::quadtree::find_in_range(0, 0, 10, 10).empty());

	tfs::map::quadtree::move_creature(5, 5, 15, 15, npc1);

	BOOST_TEST(tfs::map::quadtree::find_in_range(0, 0, 10, 10).empty());

	auto creatures = 0;
	for (auto creature : tfs::map::quadtree::find_in_range(0, 0, 20, 20)) {
		BOOST_CHECK(creature == npc1);
		creatures++;
	}

	BOOST_TEST(creatures == 1);
}

BOOST_AUTO_TEST_CASE(test_move_multiple_creature)
{
	auto npc1 = new Npc("Priestess");
	tfs::map::quadtree::push_creature(5, 5, npc1);

	auto npc2 = new Npc("Scholar");
	tfs::map::quadtree::push_creature(10, 10, npc2);

	BOOST_TEST(!tfs::map::quadtree::find_in_range(0, 0, 20, 20).empty());

	tfs::map::quadtree::move_creature(5, 5, 10, 10, npc1);
	tfs::map::quadtree::move_creature(10, 10, 15, 15, npc2);

	auto creatures = 0;
	for (auto creature : tfs::map::quadtree::find_in_range(0, 0, 20, 20)) {
		BOOST_CHECK(creature == npc1 || creature == npc2);
		creatures++;
	}

	BOOST_TEST(creatures == 2);
}

BOOST_AUTO_TEST_CASE(test_move_no_creature)
{
	auto npc1 = new Npc("Innkeeper");
	tfs::map::quadtree::push_creature(5, 5, npc1);

	auto npc2 = new Npc("Bard");
	tfs::map::quadtree::push_creature(10, 10, npc2);

	BOOST_TEST(!tfs::map::quadtree::find_in_range(0, 0, 20, 20).empty());

	tfs::map::quadtree::move_creature(5, 5, 25, 25, npc1);
	tfs::map::quadtree::move_creature(10, 10, 25, 25, npc2);

	BOOST_TEST(tfs::map::quadtree::find_in_range(0, 0, 20, 20).empty());
}

BOOST_AUTO_TEST_CASE(test_remove_creature)
{
	auto npc = new Npc("Fisherman");
	tfs::map::quadtree::push_creature(5, 5, npc);

	BOOST_TEST(!tfs::map::quadtree::find_in_range(0, 0, 10, 10).empty());

	tfs::map::quadtree::remove_creature(5, 5, npc);

	BOOST_TEST(tfs::map::quadtree::find_in_range(0, 0, 10, 10).empty());
}
