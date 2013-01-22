#ifndef CONSTRAINTS_ASSERTION_FAILURE_H_INCLUDED
#define CONSTRAINTS_ASSERTION_FAILURE_H_INCLUDED

#include "../source/assertions.h"

namespace casmine
{
    namespace constraints
    {
        namespace definitions
        {
            template <>
            struct equals<assertion::failure>
            {
                equals(assertion::failure expected) : expected(expected) { }
                assertion::failure expected;
                constraint::result<assertion::failure> operator()(assertion::failure actual) const
                {
                    if (actual.file != expected.file)
                        return constraint::failed<assertion::failure>("The file for the assertion::failure was different.", "\"" + expected.file + "\"", "\"" + actual.file + "\"");
                    if (actual.line != expected.line)
                        return constraint::failed<assertion::failure>("The line for the assertion::failure was different.", to::string(expected.line), to::string(actual.line));
                    if (actual.description != expected.description)
                        return constraint::failed<assertion::failure>("The description for the assertion::failure was different.", "\"" + expected.description + "\"", "\"" + actual.description + "\"");
                    if (actual.message != expected.message)
                        return constraint::failed<assertion::failure>("The message for the assertion::failure was different.", "\"" + expected.message + "\"", "\"" + actual.message + "\"");
                    if (actual.description != expected.description)
                        return constraint::failed<assertion::failure>("The description for the assertion::failure was different.", "\"" + expected.description + "\"", "\"" + actual.description + "\"");
                    if (actual.expected != expected.expected)
                        return constraint::failed<assertion::failure>("The expected string for the assertion::failure was different.", "\"" + expected.expected + "\"", "\"" + actual.expected + "\"");
                    if (actual.actual != expected.actual)
                        return constraint::failed<assertion::failure>("The actual string for the assertion::failure was different.", "\"" + expected.actual + "\"", "\"" + actual.actual + "\"");
                    else
                        return constraint::succeeded(actual);
                }
            };
        }
    }
}
#endif // CONSTRAINTS_ASSERTION_FAILURE_H_INCLUDED
