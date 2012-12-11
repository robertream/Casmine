#include <functional>
#include <iostream>
#include <vector>

#include "constraints.assertion.failure.h"
#include "../source/assertions.h"
#include "../source/fixtures.h"

using namespace casmine;

namespace assert_throws
{
    fixture describe("assert::throws tests",
        test("exact type of exception", []
        {
            auto throws = [] { throw 1; };
            assert::throws<int>(throws);
        })
        +
        test("no exception", []
        {
            auto no_throws = [] { };
            auto action = [=]{ assert::throws<int>(no_throws); };
            assert::throws(assertion::failure("Did not throw an exception.", "exception of type: int", "no exception"), action);
        })
        +
        test("an exception of a different type", []
        {
            auto throws = [] { throw ::std::string("different type"); };
            auto action = [=]{ assert::throws<int>(throws); };
            assert::throws(assertion::failure("An exception of an un-expected type was thrown.", "exception of type: int", "unknown exception"), action);
        })
    );
}