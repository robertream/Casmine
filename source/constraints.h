#ifndef CONSTRAINTS_H_INCLUDED
#define CONSTRAINTS_H_INCLUDED

#include <string>
#include <exception>

#include "traits.h"
#include "print.h"

namespace casmine
{
	namespace constraints
	{
		struct constraint
		{
			struct failure
			{
				failure() { }

				failure(const ::std::string& description, const ::std::string& expected, const ::std::string& actual)
					: description(description), expected(expected), actual(actual) { }

				::std::string description;
				::std::string expected;
				::std::string actual;
			};

			template <typename TValue>
			struct result
			{
				result(const TValue& value)
					: is_failure(false), value(value) { }

				result(const failure& error)
					: is_failure(true), error(error) { }

				result(const ::std::string& description, const ::std::string& expected, const ::std::string& actual)
					: is_failure(true), error(description, expected, actual) { }

				bool is_failure;
				TValue value;
				failure error;
			};

			template <typename TValue>
			static result<TValue> succeeded(const TValue& value) { return result<TValue>(value); }
			template <typename TValue>
			static result<TValue> failed(const failure& error) { return result<TValue>(error); }
			template <typename TValue>
			static result<TValue> failed(const ::std::string description, ::std::string expected, ::std::string actual) { return result<TValue>(description, expected, actual); }
			template <typename TValue>
			static result<TValue> failed(const result<TValue>& result) { return result; }

			template <typename TResultValue>
			struct of
			{
				typedef typename TResultValue result_value_type;
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
		template <typename TExpected, bool CanIterate = traits::has_const_iterator<TExpected>::value>
		struct equals_constraint;

		template <typename TExpected>
		struct equals_constraint<TExpected, false> : constraint::of<TExpected>
		{
			TExpected expected;
			equals_constraint(const TExpected& expected) : expected(expected) { }
			template<typename TActual>
			constraint::result<TActual> operator()(const TActual& actual)
			{
				return expected == actual
					? constraint::succeeded(actual)
					: constraint::failed<TActual>("The values are not equal.", to::string(expected), to::string(actual));
			}
		};

		template <typename TExpected>
		struct equals_constraint<TExpected, true> : constraint::of<TExpected>
		{
			TExpected expected;
			equals_constraint(const TExpected& expected) : expected(expected) { }
			template<typename TActual>
			constraint::result<TActual> operator()(const TActual& actual)
			{
				if (actual.size() != expected.size())
					return constraint::failed<TActual>("The sequences have different lengths.", to::string(expected), to::string(actual));

				auto actual_iterator = actual.begin();
				auto expected_iterator = expected.begin();
				while(actual_iterator != actual.end() && expected_iterator != expected.end())
				{
					if (*expected_iterator != *actual_iterator)
						return constraint::failed<TActual>("The sequences are the same length, but are not equal.", to::string(expected), to::string(actual));
					actual_iterator++;
					expected_iterator++;
				}

				return constraint::succeeded(actual);
			}
		};

		template <typename TExpected, bool CanIterate = traits::has_const_iterator<TExpected>::value>
		struct equivalent_constraint;

        template <typename TExpected>
		struct equivalent_constraint<TExpected, true> : constraint::of<TExpected>
		{
			TExpected expected;
			equivalent_constraint(const TExpected& expected) : expected(expected) { }
			template<typename TActual>
			constraint::result<TActual> operator()(const TActual& actual)
			{
				if (actual.size() != expected.size())
					return constraint::failed<TActual>("The sequences have different lengths.", to::string(expected), to::string(actual));

				::std::list<TActual::value_type> actual_to_compare(actual.begin(), actual.end());
				for_each (expected.begin(), expected.end(), [&] (auto expected_value)
					{
						auto actual_matching_value = find(actual_to_compare.begin(), actual_to_compare.end(), expected_value);
						if (actual_matching_value == actual_to_compare.end())
							return constraint::failed<TActual>("The sequences are the same length, but are not equivalent.", to::string(expected), to::string(actual));
						actual_to_compare.erase(actual_matching_value);
					});

				return constraint::succeeded(actual);
			}
		};

        template <typename TExpected>
		struct throws_constraint : constraint::of<TExpected>
		{
			template <typename TAction>
			constraint::result<TExpected> operator()(const TAction& action)
			{
				auto expected = "exception of type: " + ::std::string(typeid(TExpected).name());
				try
				{
					action();
				}
				catch(const TExpected& actual)
				{
					return constraint::succeeded(actual);
				}
				catch(const ::std::exception& actual)
				{
					return constraint::failed<TExpected>("An exception of an un-expected type was thrown.", expected, "exception of type: " + ::std::string(typeid(actual).name()) + " = " + actual.what());
				}
	            catch(...)
	            {
	                return constraint::failed<TExpected>("An exception of an un-expected type was thrown.", expected, "unknown exception");
	            }
				return constraint::failed<TExpected>("Did not throw an exception.", expected, "no exception");
			}
		};

		template <typename TConstraintA, typename TConstraintB, bool HasResultValue = traits::has_result_value<TConstraintB>::value>
		struct bind_constraint;

		template <typename TConstraintA, typename TConstraintB>
		struct bind_constraint<TConstraintA, TConstraintB, true> : constraint::from<TConstraintB>
		{
			bind_constraint(TConstraintA A, TConstraintB B)
				: A(A), B(B) { }

			TConstraintA A;
			TConstraintB B;

			template<typename TActual>
			constraint::result<result_value_type> operator()(TActual actual)
			{
				auto result = A(actual);
				return result.is_failure
						? constraint::failed<result_value_type>(result)
						: B(result.value);
			}
		};
	}

	template <typename TConstraintA, typename TConstraintB>
	static constraints::bind_constraint<TConstraintA, TConstraintB> operator && (TConstraintA A, TConstraintB B) { return constraints::bind_constraint<TConstraintA,TConstraintB>(A, B); }
}

#endif // CONSTRAINTS_H_INCLUDED