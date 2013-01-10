#ifndef CONSTRAINTS_H_INCLUDED
#define CONSTRAINTS_H_INCLUDED

#include <string>
#include <list>
#include <algorithm>
#include <exception>

#include "traits.h"
#include "print.h"
#include "typename.h"

namespace casmine
{
    namespace constraints
    {
        struct constraint
        {
            struct failure
            {
                failure() { }

                failure(::std::string description, ::std::string expected, ::std::string actual)
                    : description(description), expected(expected), actual(actual) { }

                ::std::string description;
                ::std::string expected;
                ::std::string actual;
            };

            template <typename TValue>
            struct result
            {
                result(TValue value)
                    : is_failure(false), value(value) { }

                result(failure error)
                    : is_failure(true), error(error) { }

                result(::std::string description, ::std::string expected, ::std::string actual)
                    : is_failure(true), error(description, expected, actual) { }

                bool is_failure;
                TValue value;
                failure error;
            };

            template <typename TValue>
            static result<TValue> succeeded(TValue value) { return result<TValue>(value); }
            template <typename TValue>
            static result<TValue> failed(failure error) { return result<TValue>(error); }
            template <typename TValue>
            static result<TValue> failed(::std::string description, ::std::string expected, ::std::string actual) { return result<TValue>(description, expected, actual); }
            template <typename TValue>
            static result<TValue> failed(result<TValue> result) { return result; }

            template <typename TResultValue>
            struct of
            {
                typedef TResultValue result_value_type;
            };

            template <typename TConstraint>
            struct from
            {
                typedef typename TConstraint::result_value_type result_value_type;
            };
        };
    }

    namespace constraints
    {
        template <typename TExpected, typename Enable = void>
        struct equals_constraint
        {
            typedef typename constraint::of<TExpected>::result_value_type result_value_type;

            TExpected expected;
            equals_constraint(TExpected expected) : expected(expected) { }
            template<typename TActual>
            constraint::result<TActual> operator()(TActual actual) const
            {
                return expected == actual
                    ? constraint::succeeded(actual)
                    : constraint::failed<TActual>("The values are not equal.", to::string(expected), to::string(actual));
            }
        };

        template <typename TExpected>
        struct equals_constraint<TExpected, typename ::std::enable_if<traits::has_const_iterator<TExpected>::value>::type>
        {
            typedef typename constraint::of<TExpected>::result_value_type result_value_type;

            TExpected expected;
            equals_constraint(TExpected expected) : expected(expected) { }
            template<typename TActual>
            constraint::result<TActual> operator()(TActual actual) const
            {
                if (actual.size() != expected.size())
                    return constraint::failed<TActual>("The collections have different lengths.", to::string(expected), to::string(actual));

                auto actual_iterator = actual.begin();
                auto expected_iterator = expected.begin();
                while(actual_iterator != actual.end() && expected_iterator != expected.end())
                {
                    if (*expected_iterator != *actual_iterator)
                        return constraint::failed<TActual>("The collections are the same length, but are not equal.", to::string(expected), to::string(actual));
                    actual_iterator++;
                    expected_iterator++;
                }

                return constraint::succeeded(actual);
            }
        };

        template <typename TExpected, typename Enable = void>
        struct equivalent_constraint;

        template <typename TExpected>
        struct equivalent_constraint<TExpected, typename ::std::enable_if<traits::has_const_iterator<TExpected>::value>::type>
        {
            typedef typename constraint::of<TExpected>::result_value_type result_value_type;

            TExpected expected;
            equivalent_constraint(TExpected expected) : expected(expected) { }
            template<typename TActual>
            constraint::result<TActual> operator()(TActual actual) const
            {
                if (actual.size() != expected.size())
                    return constraint::failed<TActual>("The collections have different lengths.", to::string(expected), to::string(actual));

                ::std::list<typename TActual::value_type> actual_to_compare(actual.begin(), actual.end());
                for (auto expected_iterator = expected.begin(); expected_iterator != expected.end(); expected_iterator++)
                {
                    auto actual_matching_value = ::std::find(actual_to_compare.begin(), actual_to_compare.end(), *expected_iterator);
                    if (actual_matching_value == actual_to_compare.end())
                        return constraint::failed<TActual>("The collections are the same length, but have different values.", to::string(expected), to::string(actual));
                    actual_to_compare.erase(actual_matching_value);
                };

                return constraint::succeeded(actual);
            }
        };

        template <typename TExpected>
        struct throws_constraint
        {
            typedef typename constraint::of<TExpected>::result_value_type result_value_type;

            template <typename TAction>
            constraint::result<TExpected> operator()(TAction action) const
            {
                auto expected = "exception of type: " + typename_of<TExpected>();
                try
                {
                    action();
                }
                catch(TExpected actual)
                {
                    return constraint::succeeded(actual);
                }
                catch(::std::exception actual)
                {
                    return constraint::failed<TExpected>("An exception of an un-expected type was thrown.", expected, "exception of type: " + typename_of(actual) + " = " + actual.what());
                }
                catch(...)
                {
                    return constraint::failed<TExpected>("An exception of an un-expected type was thrown.", expected, "unknown exception");
                }
                return constraint::failed<TExpected>("Did not throw an exception.", expected, "no exception");
            }
        };

        template <typename TConstraintA, typename TConstraintB, typename Enabled = void>
        struct and_bind_constraint;

        template <typename TConstraintA, typename TConstraintB>
        struct and_bind_constraint<TConstraintA, TConstraintB, typename ::std::enable_if<traits::has_result_value<TConstraintB>::value>::type>
        {
            typedef typename constraint::from<TConstraintB>::result_value_type result_value_type;

            and_bind_constraint(TConstraintA A, TConstraintB B)
                : A(A), B(B) { }

            TConstraintA A;
            TConstraintB B;

            template<typename TActual>
            constraint::result<result_value_type> operator()(TActual actual) const
            {
                auto result = A(actual);
                return result.is_failure
                        ? constraint::failed<result_value_type>(result.error)
                        : B(result.value);
            }
        };

        template <typename TConstraintA, typename TConstraintB>
        static and_bind_constraint<TConstraintA, TConstraintB> and_bind(TConstraintA A, TConstraintB B) { return and_bind_constraint<TConstraintA,TConstraintB>(A, B); }
    }

    template <typename TConstraintA, typename TConstraintB>
    static constraints::and_bind_constraint<TConstraintA, TConstraintB> operator >>=(TConstraintA A, TConstraintB B) { return constraints::and_bind(A, B); }
}

#endif // CONSTRAINTS_H_INCLUDED
