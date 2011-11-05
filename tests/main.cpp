#include <functional>
#include <iostream>

#include "../source/assertions.h"
#include "../source/conversions.h"

using namespace casmine;

struct test
{
    test(::std::string description, ::std::function<void (void)> run_test)
        : description(description), run_test(run_test) { }

    ::std::string description;
    ::std::function<void (void)> run_test;
};

struct fixture
{
    fixture(const ::std::string description, const test& first_test)
        : description(description)
    {
        tests.push_back(first_test);
    }

    fixture(const ::std::string description, const fixture& source_fixture)
        : description(description), tests(source_fixture.tests) { }

    ::std::string description;
    ::std::vector<test> tests;

    int run()
    {
        int failures = 0;
        for(auto current_test = tests.begin(); current_test != tests.end(); current_test++)
        try
        {
            ::std::cout << current_test->description;
            current_test->run_test();
            ::std::cout << " - passed." << ::std::endl;
        }
        catch(::std::string e)
        {
            failures++;
            ::std::cout << " - failed." << ::std::endl;
            ::std::cout << "  " << e << ::std::endl;
        }
        catch(...)
        {
            failures++;
            ::std::cout << " - failed." << ::std::endl;
            ::std::cout << "  *unexpected exception*" << ::std::endl;
        }
        return failures;
    }
};


fixture operator +(const test& first_test, const test& test_to_add)
{
    fixture result("", first_test);
    result.tests.push_back(test_to_add);
    return result;
}

fixture operator +(const fixture source_fixture, const test& test_to_add)
{
    fixture result(source_fixture.description, source_fixture);
    result.tests.push_back(test_to_add);
    return result;
}

fixture describe("assert::are_equal tests",
    test("assert::are_equal and the values are not equal", []
    {
        auto action = []{ assert::are_equal(1, 2); };
        assert::throws(assertion_failure("The values are not equal.", "1", "2"), action);
    })

    +test("assert::are_equal with the same values", []
    {
        assert::are_equal(1, 1);
    })

    +test("assert::are_equal with the same sequence",[]
    {
        assert::are_equal(sequence(1, 2), sequence(1, 2));
    })

    +test("assert::are_equivalent with the same sequence values but different order",[]
    {
        auto action = []{ assert::are_equal(sequence(1, 2), sequence(2, 1)); };
        assert::throws(assertion_failure("The sequences are the same length, but are not equal.", "[2] { 1, 2 }", "[2] { 2, 1 }"), action);
    })

    +test("assert::are_equal with sequences of different lengths", []
    {
        auto action = []{ assert::are_equal(sequence(1), sequence<int>()); };
        assert::throws(assertion_failure("The sequences have different lengths.", "[1] { 1 }", "[0] { }"), action);
    })

    +test("assert::are_equal with sequences of different values", []
    {
        auto action = []{ assert::are_equal(sequence(1, 2), sequence(3, 4)); };
        assert::throws(assertion_failure("The sequences are the same length, but are not equal.", "[2] { 1, 2 }", "[2] { 3, 4 }"), action);
    })

    +test("assert::are_equal with the same bool values", []
    {
        assert::are_equal(true, true);
        assert::are_equal(false, false);
    })

    +test("assert::are_equal with different boolean values", []
    {
        auto action = []{ assert::are_equal(true, false); };
        assert::throws(assertion_failure("The values are not equal.", "true", "false"), action);
    })

    +test("assert::are_equivalent with the same sequence",[]
    {
        assert::are_equivalent(sequence(1, 2), sequence(1, 2));
    })

    +test("assert::are_equivalent with the same sequence values but different order",[]
    {
        assert::are_equivalent(sequence(1, 2, 3), sequence(2, 1, 3));
    })

    +test("assert::are_equivalent with sequences of different lengths", []
    {
        auto action = []{ assert::are_equivalent(sequence(1), sequence<int>()); };
        assert::throws(assertion_failure("The sequences have different lengths.", "[1] { 1 }", "[0] { }"), action);
    })

    +test("assert::are_equivalent with sequences of different values", []
    {
        auto action = []{ assert::are_equivalent(sequence(1, 2), sequence(3, 4)); };
        assert::throws(assertion_failure("The sequences are the same length, but are not equivalent.", "[2] { 1, 2 }", "[2] { 3, 4 }"), action);
    })

    +test("assert::throws with a specific type", []
    {
        auto throws = [] { throw 1; };
        assert::throws<int>(throws);
    })

    +test("assert::throws with an exception of a different type", []
    {
        auto no_throws = [] { };
        auto action = [=]{ assert::throws<int>(no_throws); };
        assert::throws(assertion_failure("Did not throw an exception.", "An exception of a specific type.", "No exception."), action);
    })

    +test("assert::throws with an exception of a different type", []
    {
        auto throws = [] { throw ::std::string("different type"); };
        auto action = [=]{ assert::throws<int>(throws); };
        assert::throws(assertion_failure("Threw an exception of an un-expected type.", "An exception of a specific type.", "An exception of a different type."), action);
    })
);

int main()
{
    return describe.run();
}
