#ifndef ASSERTIONS_H_INCLUDED
#define ASSERTIONS_H_INCLUDED

#include <string>
#include <sstream>
#include <list>
#include <algorithm>
#include <typeinfo>

#include "assertion_failure.h"

namespace casmine
{
	template <typename T>
	struct has_const_iterator
	{
		private:
			template <typename TTest>
			static typename TTest::const_iterator test(int);
			template <typename>
			static void test(...);
		public:
			enum { value = !::std::is_void<decltype(test<T>(0))>::value };
	};

    namespace to
    {
		template <typename T>
		::std::string string(const T& value) { return print::string<T>(value); }
	}

	namespace print
	{
		template <typename T, bool CanIterate = has_const_iterator<T>::value>
		struct string;

        template <typename T>
		struct string<T, false> : ::std::string
		{
			string(T value)
			{
				::std::stringstream string_value;
				string_value << value;
				this->assign(string_value.str());
			}
		};

        template <>
		struct string<bool, false> : ::std::string
		{
			string(bool value) { this->assign(value == true ? "true" : "false"); }
		};

        template <>
		struct string<char*, false> : ::std::string
		{
			string(char* value) { this->assign(value); }
		};

        template <typename T>
		struct string<T, true> : ::std::string
		{
			string(const T& value)
			{
				::std::stringstream string_value;
				if (value.size() == 0)
				{
					string_value << "[0] { }";
				}
				else
				{
					string_value << '[' << value.size() << ']';
					string_value << " { ";
					auto item = value.begin();
					string_value << to::string(*item);
					while (++item != value.end())
						string_value  << ", " << to::string(*item);
					string_value << " }";
				}
				this->assign(string_value.str());
			}
		};
	}

    namespace assert
    {
		template <typename TExpected, bool CanIterate = has_const_iterator<TExpected>::value>
		struct equals_constraint;

        template <typename TExpected>
		struct equals_constraint<TExpected, false>
		{
			TExpected expected;
			equals_constraint(const TExpected& expected) : expected(expected) { }
			template<typename TActual>
			void compare(const TActual& actual)
			{
				if (expected != actual)
					throw assertion_failure("The values are not equal.", to::string(expected), to::string(actual));
			}
		};

        template <typename TExpected>
		struct equals_constraint<TExpected, true>
		{
			TExpected expected;
			equals_constraint(const TExpected& expected) : expected(expected) { }
			template<typename TActual>
			void compare(const TActual& actual)
			{
				if (actual.size() != expected.size())
					throw assertion_failure("The sequences have different lengths.", to::string(expected), to::string(actual));

				auto actual_iterator = actual.begin();
				auto expected_iterator = expected.begin();
				while(actual_iterator != actual.end() && expected_iterator != expected.end())
				{
					if (*expected_iterator != *actual_iterator)
						throw assertion_failure("The sequences are the same length, but are not equal.", to::string(expected), to::string(actual));
					actual_iterator++;
					expected_iterator++;
				}
			}
		};

		template <typename TExpected, typename TActual>
        void are_equal(const TExpected& expected, const TActual& actual)
        {
			equals_constraint<TExpected>(expected).compare(actual);
        }

		template <typename TExpected, bool CanIterate = has_const_iterator<TExpected>::value>
		struct equivalent_constraint;

        template <typename TExpected>
		struct equivalent_constraint<TExpected, true>
		{
			TExpected expected;
			equivalent_constraint(const TExpected& expected) : expected(expected) { }
			template<typename TActual>
			void compare(const TActual& actual)
			{
				if (actual.size() != expected.size())
					throw assertion_failure("The sequences have different lengths.", to::string(expected), to::string(actual));

				::std::list<TActual::value_type> actual_to_compare(actual.begin(), actual.end());
				for_each (expected.begin(), expected.end(), [&] (auto expected_value)
					{
						auto actual_matching_value = find(actual_to_compare.begin(), actual_to_compare.end(), expected_value);
						if (actual_matching_value == actual_to_compare.end())
							throw assertion_failure("The sequences are the same length, but are not equivalent.", to::string(expected), to::string(actual));
						actual_to_compare.erase(actual_matching_value);
					});
			}
		};

		template <typename TExpected, typename TActual>
        void are_equivalent(const TExpected& expected, const TActual& actual)
        {
			equivalent_constraint<TExpected>(expected).compare(actual);
        }

        template <typename TExpected>
		struct throws_constraint
		{
			template <typename TAction>
			void compare(TAction action)
			{
				auto expected = "exception of type: " + ::std::string(typeid(TExpected).name());
				try
				{
					action();
				}
				catch(TExpected&)
				{
					return;
				}
				catch(::std::exception& actual)
				{
					throw assertion_failure("Threw an exception of an un-expected type.", expected, "exception of type: " + ::std::string(typeid(actual).name()) + " = " + actual.what());
				}
	            catch(...)
	            {
	                throw assertion_failure("Threw an exception of an un-expected type.", expected, "unknown exception");
	            }
				throw assertion_failure("Did not throw an exception.", expected, "no exception");
			}
		};

        template <typename TExpected, typename TAction>
        void throws(TAction action)
        {
			throws_constraint<TExpected>().compare(action);
        }
	}
}

#endif // ASSERTIONS_H_INCLUDED
