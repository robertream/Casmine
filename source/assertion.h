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
            failure() : line(0) { }

            failure(::std::string description, ::std::string expected, ::std::string actual)
                : file(""), line(0), message(""), description(description), expected(expected), actual(actual) { }

            failure(::std::string file, int line, ::std::string message, ::std::string description, ::std::string expected, ::std::string actual)
                : file(file), line(line), message(message), description(description), expected(expected), actual(actual) { }

            ::std::string file;
            int line;
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
            comparison(::std::string file, int line, TActual actual, TConstraint constraint)
                : file(file), line(line), actual(actual), constraint(constraint) { }

            ::std::string file;
            int line;
            TActual actual;
            TConstraint constraint;

            void operator()(const char* message) const
            {
                auto result = constraint(actual);
                if (result.is_failure)
                    throw assertion::failure(file, line, message, result.error.description, result.error.expected, result.error.actual);
            }
        };

        template<typename TActual>
        struct actual
        {
            actual(::std::string file, int line, TActual value)
                : file(file), line(line), value(value) { }

            ::std::string file;
            int line;
            TActual value;

            template<typename TConstraint>
            comparison<TActual, TConstraint> operator()(TConstraint constraint) const
            {
                return comparison<TActual, TConstraint>(file, line, value, constraint);
            }
        };
    }
}

#endif // ASSERTION_H_INCLUDED
