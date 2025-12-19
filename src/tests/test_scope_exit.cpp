#define BOOST_TEST_MODULE scope_exit

#include "../otpch.h"

#include "../tools.h"

#include <boost/test/unit_test.hpp>

// Test that the scope_exit destructor executes the cleanup function
// when the scope is exited normally.
BOOST_AUTO_TEST_CASE(test_scope_exit_runs_on_destruction)
{
	bool ran = false;
	{
		// Create a scope_exit with a lambda that sets ran to true
		tfs::scope_exit se{[&] { ran = true; }};
		// Verify the lambda hasn't run yet
		BOOST_TEST(!ran);
		// Exit the scope, triggering the destructor
	}
	// Verify the cleanup lambda was executed
	BOOST_TEST(ran);
}

// Test that scope_exit executes cleanup even when a function
// returns early, ensuring RAII behavior works correctly.
BOOST_AUTO_TEST_CASE(test_scope_exit_with_early_return)
{
	bool ran = false;
	auto func = [&]() -> int {
		tfs::scope_exit se{[&] { ran = true; }};
		// Return early from the function
		if (!ran) {
			return 42;
		}

		std::unreachable();
	};

	int v = func();
	// Verify the function returned correctly
	BOOST_TEST(v == 42);
	// Verify cleanup ran even with early return
	BOOST_TEST(ran);
}

// Test that scope_exit executes cleanup when an exception is thrown,
// verifying that stack unwinding properly triggers the destructor.
BOOST_AUTO_TEST_CASE(test_scope_exit_on_exception)
{
	bool ran = false;
	try {
		// Create a scope_exit that sets ran to true
		tfs::scope_exit se{[&] { ran = true; }};
		// Throw an exception to trigger stack unwinding
		throw std::runtime_error("boom");
		// scope_exit destructor runs during stack unwinding
	} catch (const std::exception& e) {
		// Verify cleanup ran before exception was caught
		BOOST_TEST(ran);
		// Verify we caught the correct exception
		BOOST_TEST(std::string(e.what()) == "boom");
	}
}
