#ifndef ASSERTIONS_H_INCLUDED
#define ASSERTIONS_H_INCLUDED

#include <string>
#include <sstream>
#include <list>
#include <algorithm>

namespace casmine
{
    class assertion_failure
    {
    public:
        assertion_failure(const ::std::string& description, const ::std::string& expected, const ::std::string& actual)
            : description(description), expected(expected), actual(actual) { }

        ::std::string description;
        ::std::string expected;
        ::std::string actual;
    };

    namespace to
    {
        template <typename T>
        ::std::string string(T value)
        {
            ::std::stringstream string_value;
            string_value << value;
            return string_value.str();
        }

        template <>
        ::std::string string(bool value) { return value == true ? "true" : "false"; }

        template <>
        ::std::string string(char* value) { return ::std::string(value); }

        template <typename T>
        ::std::string string(::std::list<T> value)
        {
            if (value.size() == 0)
                return "[0] { }";

            ::std::stringstream string_value;
            string_value << '[' << value.size() << ']';
            string_value << " { ";
            auto item = value.begin();
            string_value << string(*item);
            while (++item != value.end())
                string_value  << ", " << string(*item);
            string_value << " }";
            return string_value.str();
        }
    }

    namespace assert
    {
        template <typename T>
        void are_equal(::std::list<T> expected, ::std::list<T> actual)
        {
            if (actual.size() != expected.size())
                throw assertion_failure("The sequences have different lengths.", to::string(expected), to::string(actual));

            auto actual_iterator = actual.begin();
            auto expected_iterator = expected.begin();
            while(actual_iterator != actual.end() && expected_iterator != expected.end())
            {
                if (*expected_iterator != *actual_iterator)
                    throw assertion_failure("The sequences are the same length, but are not equal.", to::string(expected), to::string(actual));
                actual_iterator++;
                expected_iterator++;
            }
        }

        template <typename T>
        void are_equal(T expected, T actual)
        {
            if (expected != actual)
                throw assertion_failure("The values are not equal.", to::string(expected), to::string(actual));
        }

        template <typename T>
        void are_equivalent(::std::list<T> expected, ::std::list<T> actual)
        {
            if (actual.size() != expected.size())
                throw assertion_failure("The sequences have different lengths.", to::string(expected), to::string(actual));

            ::std::list<T> actual_to_compare(actual);
            for(auto expected_iterator = expected.begin(); expected_iterator != expected.end(); expected_iterator++)
            {
                auto actual_matching_value = find(actual_to_compare.begin(), actual_to_compare.end(), *expected_iterator);
                if (actual_matching_value == actual_to_compare.end())
                    throw assertion_failure("The sequences are the same length, but are not equivalent.", to::string(expected), to::string(actual));
                actual_to_compare.erase(actual_matching_value);
            }
        }

        template <typename TException>
        void throws(::std::function<void (void)> action)
        {
            try
            {
                action();
            }
            catch(TException e)
            {
                return;
            }
            catch(...)
            {
                throw assertion_failure("Threw an exception of an un-expected type.", "An exception of a specific type.", "An exception of a different type.");
            }
            throw assertion_failure("Did not throw an exception.", "An exception of a specific type.", "No exception.");
        }

        void throws(assertion_failure expected, ::std::function<void (void)> action)
        {
            try
            {
                action();
            }
            catch(const assertion_failure actual)
            {
                if (actual.description != expected.description)
                    throw ::std::string("assertion_failure description string expected: " + expected.description + " actual: " + actual.description);
                if (actual.expected != expected.expected)
                    throw ::std::string("assertion_failure expected string expected: " + expected.expected + " actual: " + actual.expected);
                if (actual.actual != expected.actual)
                    throw ::std::string("assertion_failure actual string expected: " + expected.actual + " actual: " + actual.actual);
                return;
            }
            throw ::std::string("expected an assertion_failure exception");
        }
    }
}

#endif // ASSERTIONS_H_INCLUDED
