#ifndef ASSERTIONS_H_INCLUDED
#define ASSERTIONS_H_INCLUDED

#include <list>
#include <algorithm>
#include <typeinfo>

#include "assertion_failure.h"
#include "constraints.h"

namespace casmine
{
    namespace assert
    {
		template <typename TExpected, typename TActual>
        void are_equal(const TExpected& expected, const TActual& actual)
        {
			compare(constraints::equals_constraint<TExpected>(expected), actual);
        }

		template <typename TExpected, typename TActual>
        void are_equivalent(const TExpected& expected, const TActual& actual)
        {
			compare(constraints::equivalent_constraint<TExpected>(expected), actual);
        }

        template <typename TExpected, typename TAction>
        void throws(TAction action)
        {
			compare(constraints::throws_constraint<TExpected>(), action);
        }
	}
}

#endif // ASSERTIONS_H_INCLUDED
