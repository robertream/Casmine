#ifndef CHECK_H_INCLUDED
#define CHECK_H_INCLUDED

#include "assertion.h"
#include "constraints.h"

namespace casmine
{
    namespace check
    {
        template <typename TActual>
        assertion::actual<TActual> that(TActual actual, const char* file = "", int line = 0)
        {
            return assertion::actual<TActual>(file, line, actual);
        }
    }

    template <typename TAssertion>
    const char* operator -(const char* message, TAssertion assertion)
    {
        assertion(message);
        return message;
    }

    template <typename TActual, typename TExpected>
    auto operator == (assertion::actual<TActual> actual, TExpected expected) -> decltype(actual(is::equal_to(expected)))
    {
        return actual(is::equal_to(expected));
    }
    
    #define CHECK_THAT(_actual, _constraint) check::that(_actual, __FILE__, __LINE__)(_constraint)
    #define CHECK(_actual) check::that(_actual, __FILE__, __LINE__)
}

#endif // CHECK_H_INCLUDED
