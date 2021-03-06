#include "../source/casmine.h"
#include "../source/sequence.h"
#include "constraints.assertion.failure.h"

using namespace std;

namespace assert_are_equal
{
    fixture describe("assert::are_equal tests",
        test("values that are not equal", []
        {
            auto action = []{ assert::are_equal(1, 2); };
            assert::throws(assertion::failure("The values are not equal.", "1", "2"), action);
        })
        +
        test("values that are equal", []
        {
            assert::are_equal(1, 1);
        })
        +
        test("collections of different types", []
        {
            assert::are_equal(vector<int>(), list<int>());
        })
        +
        test("collections of the same values",[]
        {
            assert::are_equal(sequence(1, 2), sequence(1, 2));
        })
        +
        test("collections of different lengths", []
        {
            auto action = []{ assert::are_equal(sequence(1), sequence<int>()); };
            assert::throws(assertion::failure("The collections have different lengths.", "[1] { 1 }", "[0] { }"), action);
        })
        +
        test("collections of different values", []
        {
            auto action = []{ assert::are_equal(sequence(1, 2), sequence(3, 4)); };
            assert::throws(assertion::failure("The collections are the same length, but are not equal.", "[2] { 1, 2 }", "[2] { 3, 4 }"), action);
        })
        +
        test("the same bool values", []
        {
            assert::are_equal(true, true);
            assert::are_equal(false, false);
        })
        +
        test("different boolean values", []
        {
            auto action = []{ assert::are_equal(true, false); };
            assert::throws(assertion::failure("The values are not equal.", "true", "false"), action);
        })
    );
}
