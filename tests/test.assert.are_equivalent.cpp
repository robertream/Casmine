#include <functional>
#include <iostream>
#include <vector>

#include "constraints.assertion_failure.h"
#include "../source/assertions.h"
#include "../source/fixtures.h"
#include "../source/sequence.h"

using namespace casmine;
using namespace ::std;


namespace assert_are_equivalent
{
	fixture describe("assert::are_equivalent tests",
		test("collections of different types", []
		{
			assert::are_equivalent(vector<int>(), list<int>());
		})
		+
		test("collections of the same values",[]
		{
			assert::are_equivalent(sequence(1, 2), sequence(1, 2));
		})
		+
		test("collections of the same values but different order",[]
		{
			assert::are_equivalent(sequence(2, 1), sequence(1, 2));
		})
		+
		test("collections of different lengths", []
		{
			auto action = []{ assert::are_equivalent(sequence(1), sequence(1, 1)); };
			assert::throws(assertion_failure("The collections have different lengths.", "[1] { 1 }", "[2] { 1, 1 }"), action);
		})
		+
		test("collections of different values", []
		{
			auto action = []{ assert::are_equivalent(sequence(1, 2), sequence(3, 4)); };
			assert::throws(assertion_failure("The collections are the same length, but have different values.", "[2] { 1, 2 }", "[2] { 3, 4 }"), action);
		})
	);
}
