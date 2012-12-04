#include <functional>
#include <iostream>
#include <vector>

#pragma warning( disable : 4290 )

#include "assert.assertion_failure.h"
#include "../source/assertions.h"
#include "../source/fixtures.h"

using namespace casmine;

namespace assert_throws
{
	fixture describe("assert::throws tests",
		test("with a specific type", []
		{
			auto throws = [] { throw 1; };
			assert::throws<int>(throws);
		})

		+test("with an exception of a different type", []
		{
			auto no_throws = [] { };
			auto action = [=]{ assert::throws<int>(no_throws); };
			assert::throws(assertion_failure("Did not throw an exception.", "exception of type: int", "no exception"), action);
		})

		+test("with an exception of a different type", []
		{
			auto throws = [] { throw ::std::string("different type"); };
			auto action = [=]{ assert::throws<int>(throws); };
			assert::throws(assertion_failure("Threw an exception of an un-expected type.", "exception of type: int", "unknown exception"), action);
		})
	);
}