#ifndef ASSERTION_H_INCLUDED
#define ASSERTION_H_INCLUDED

#include <string>
#include <exception>

namespace casmine
{
    namespace assertion
    {
        struct failure : ::std::exception
        {
            failure() { }

            failure(const ::std::string& description, const ::std::string& expected, const ::std::string& actual)
                : message(""), description(description), expected(expected), actual(actual) { }

            failure(const ::std::string& message, const ::std::string& description, const ::std::string& expected, const ::std::string& actual)
                : message(message), description(description), expected(expected), actual(actual) { }

            ::std::string message;
            ::std::string description;
            ::std::string expected;
            ::std::string actual;
        };

        template<typename TActual, typename TConstraint>
        struct comparison
        {
            comparison(TActual actual, TConstraint constraint)
                : actual(actual), constraint(constraint) { }

            TActual actual;
            TConstraint constraint;

            void operator()(const char* message) const
            {
                auto result = constraint(actual);
                if (result.is_failure)
                    throw assertion::failure(message, result.error.description, result.error.expected, result.error.actual);
            }
        };

        template<typename TActual>
        struct actual
        {
            actual(TActual value)
                : value(value) { }

            TActual value;

            template<typename TConstraint>
            comparison<TActual, TConstraint> operator()(TConstraint constraint) const
            {
                return comparison<TActual, TConstraint>(value, constraint);
            }
        };
    }
}

#endif // ASSERTION_H_INCLUDED
