#define BOOST_TEST_MODULE matrixarea

#include "../otpch.h"

#include "../matrixarea.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_createArea)
{
	// clang-format off
	auto m = createArea({
        0, 0, 1, 1,
        3, 1, 1, 1,
        0, 0, 1, 1,
    }, 3);
	// clang-format on

	auto&& [centerX, centerY] = m.getCenter();
	BOOST_TEST(centerX == 0);
	BOOST_TEST(centerY == 1);

	BOOST_TEST(m.getCols() == 4);
	BOOST_TEST(m.getRows() == 3);

	BOOST_TEST(!m(0, 0));
	BOOST_TEST(!m(0, 1));
	BOOST_TEST(m(0, 2));
	BOOST_TEST(m(0, 3));
	BOOST_TEST(m(1, 0));
	BOOST_TEST(m(1, 1));
	BOOST_TEST(m(1, 2));
	BOOST_TEST(m(1, 3));
	BOOST_TEST(!m(2, 0));
	BOOST_TEST(!m(2, 1));
	BOOST_TEST(m(2, 2));
	BOOST_TEST(m(2, 3));
}

BOOST_AUTO_TEST_CASE(test_MatrixArea_rotate90)
{
	// clang-format off
	auto m = createArea({
        3, 1, 1, 1,
        0, 1, 1, 1,
        0, 0, 1, 0,
    }, 3).rotate90();
	// clang-format on

	/** expected area:
	 * 0, 0, 3,
	 * 0, 1, 1,
	 * 1, 1, 1,
	 * 0, 1, 1,
	 */
	auto&& [centerX, centerY] = m.getCenter();
	BOOST_TEST(centerX == 2);
	BOOST_TEST(centerY == 0);

	BOOST_TEST(m.getCols() == 3);
	BOOST_TEST(m.getRows() == 4);

	BOOST_TEST(!m(0, 0));
	BOOST_TEST(!m(0, 1));
	BOOST_TEST(m(0, 2));
	BOOST_TEST(!m(1, 0));
	BOOST_TEST(m(1, 1));
	BOOST_TEST(m(1, 2));
	BOOST_TEST(m(2, 0));
	BOOST_TEST(m(2, 1));
	BOOST_TEST(m(2, 2));
	BOOST_TEST(!m(3, 0));
	BOOST_TEST(m(3, 1));
	BOOST_TEST(m(3, 2));
}

BOOST_AUTO_TEST_CASE(test_MatrixArea_rotate180)
{
	// clang-format off
	auto m = createArea({
        3, 1, 1, 1,
        0, 1, 1, 1,
        0, 0, 1, 0,
    }, 3).rotate180();
	// clang-format on

	/** expected area:
	 * 0, 1, 0, 0,
	 * 1, 1, 1, 0,
	 * 1, 1, 1, 3,
	 */
	auto&& [centerX, centerY] = m.getCenter();
	BOOST_TEST(centerX == 3);
	BOOST_TEST(centerY == 2);

	BOOST_TEST(m.getCols() == 4);
	BOOST_TEST(m.getRows() == 3);

	BOOST_TEST(!m(0, 0));
	BOOST_TEST(m(0, 1));
	BOOST_TEST(!m(0, 2));
	BOOST_TEST(!m(0, 3));
	BOOST_TEST(m(1, 0));
	BOOST_TEST(m(1, 1));
	BOOST_TEST(m(1, 2));
	BOOST_TEST(!m(1, 3));
	BOOST_TEST(m(2, 0));
	BOOST_TEST(m(2, 1));
	BOOST_TEST(m(2, 2));
	BOOST_TEST(m(2, 2));
}

BOOST_AUTO_TEST_CASE(test_MatrixArea_rotate270)
{
	// clang-format off
	auto m = createArea({
        3, 1, 1, 1,
        0, 1, 1, 1,
        0, 0, 1, 0,
    }, 3).rotate270();
	// clang-format on

	/** expected area:
	 * 1, 1, 0,
	 * 1, 1, 1,
	 * 1, 1, 0,
	 * 3, 0, 0,
	 */
	auto&& [centerX, centerY] = m.getCenter();
	BOOST_TEST(centerX == 0);
	BOOST_TEST(centerY == 3);

	BOOST_TEST(m.getCols() == 3);
	BOOST_TEST(m.getRows() == 4);

	BOOST_TEST(m(0, 0));
	BOOST_TEST(m(0, 1));
	BOOST_TEST(!m(0, 2));
	BOOST_TEST(m(1, 0));
	BOOST_TEST(m(1, 1));
	BOOST_TEST(m(1, 2));
	BOOST_TEST(m(2, 0));
	BOOST_TEST(m(2, 1));
	BOOST_TEST(!m(2, 2));
	BOOST_TEST(m(3, 0));
	BOOST_TEST(!m(3, 1));
	BOOST_TEST(!m(3, 2));
}
