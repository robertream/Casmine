#ifndef ASSERTION_FAILURE_H_INCLUDED
#define ASSERTION_FAILURE_H_INCLUDED

#include <string>

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
}

#endif // ASSERTION_FAILURE_H_INCLUDED
