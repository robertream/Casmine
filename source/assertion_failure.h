#ifndef ASSERTION_FAILURE_H_INCLUDED
#define ASSERTION_FAILURE_H_INCLUDED

#include <string>
#include <exception>

namespace casmine
{
	struct assertion_failure
    {
		assertion_failure() { }

		assertion_failure(const ::std::string& description, const ::std::string& expected, const ::std::string& actual)
            : description(description), expected(expected), actual(actual) { }

        ::std::string description;
        ::std::string expected;
        ::std::string actual;
    };

	template<typename TConstraint, typename TActual>
	void compare(TConstraint constraint, const TActual& actual)
	{
		auto result = constraint(actual);
		if (result.is_failure)
			throw assertion_failure(result.error.description, result.error.expected, result.error.actual);
	}
}

#endif // ASSERTION_FAILURE_H_INCLUDED
