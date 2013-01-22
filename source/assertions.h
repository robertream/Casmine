#ifndef ASSERTIONS_H_INCLUDED
#define ASSERTIONS_H_INCLUDED

#include "assertion.h"
#include "constraints.h"

namespace casmine
{
    namespace is
    {
        template <typename TExpected>
        auto equal_to(TExpected expected) -> decltype(constraints::equals(expected))
        {
            return constraints::equals(expected);
        }

        template <typename TExpected>
        auto equivalent_to(TExpected expected) -> decltype(constraints::equivalent(expected))
        {
            return constraints::equivalent(expected);
        }
    }

    namespace throws
    {
        template <typename TExpected>
        auto type_of() -> decltype(constraints::throws<TExpected>())
        {
            return constraints::throws<TExpected>();
        }
    }

    namespace assert
    {
        template <typename TActual, typename TConstraint>
        void that(TActual actual, TConstraint constraint)
        {
            assertion::comparison<TActual, TConstraint>("", 0, actual, constraint)("");
        }

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
    }
}

#endif // ASSERTIONS_H_INCLUDED
