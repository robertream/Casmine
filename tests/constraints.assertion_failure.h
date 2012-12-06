#ifndef ASSERT_ASSERTION_FAILURE_H_INCLUDED
#define ASSERT_ASSERTION_FAILURE_H_INCLUDED

#include "../source/assertions.h"

namespace casmine
{
	namespace constraints
	{
        template <>
		struct equals_constraint<assertion_failure> : constraint::of<assertion_failure>
		{
			assertion_failure expected;
			equals_constraint(const assertion_failure& expected) : expected(expected) { }

			constraint::result<assertion_failure> operator()(const assertion_failure& actual)
			{
				if (actual.description != expected.description)
					return constraint::failed<assertion_failure>("The description for the assertion_failure was different.", "\"" + expected.description + "\"", "\"" + actual.description + "\"");
				if (actual.expected != expected.expected)
					return constraint::failed<assertion_failure>("The expected string for the assertion_failure was different.", "\"" + expected.expected + "\"", "\"" + actual.expected + "\"");
				if (actual.actual != expected.actual)
					return constraint::failed<assertion_failure>("The actual string for the assertion_failure was different.", "\"" + expected.actual + "\"", "\"" + actual.actual + "\"");
				else
					return constraint::succeeded(actual);
			}
		};
	}
}

#endif // ASSERT_ASSERTION_FAILURE_H_INCLUDED
