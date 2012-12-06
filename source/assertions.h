#ifndef ASSERTIONS_H_INCLUDED
#define ASSERTIONS_H_INCLUDED

#include <list>
#include <algorithm>
#include <typeinfo>

#include "assertion_failure.h"
#include "constraints.h"

namespace casmine
{
	namespace is
	{
		template <typename TExpected>
        constraints::equals_constraint<TExpected> equal_to(TExpected expected)
        {
			return constraints::equals_constraint<TExpected>(expected);
        }

		template <typename TExpected>
        constraints::equivalent_constraint<TExpected> equivalent_to(TExpected expected)
        {
			return constraints::equivalent_constraint<TExpected>(expected);
        }
	}

	namespace throws
	{
		template <typename TExpected>
        constraints::throws_constraint<TExpected> type_of()
        {
			return constraints::throws_constraint<TExpected>();
        }
	}

	namespace assert
    {
		template <typename TExpected, typename TActual>
        void are_equal(TExpected expected, TActual actual)
        {
			that(actual, is::equal_to(expected));
        }

		template <typename TExpected, typename TActual>
        void are_equivalent(TExpected expected, TActual actual)
        {
			that(actual, is::equivalent_to(expected));
        }

        template <typename TExpected, typename TAction>
        void throws(TAction action)
        {
			that(action, throws::type_of<TExpected>());
        }

        template <typename TException, typename TAction>
		void throws(TException expected, TAction action)
		{
			that(action, throws::type_of<TException>() && is::equal_to(expected));
		}

		template <typename TActual, typename TConstraint>
        void that(TActual actual, TConstraint constraint)
        {
			compare(constraint, actual);
        }
	}
}

#endif // ASSERTIONS_H_INCLUDED
