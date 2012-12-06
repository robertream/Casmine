#ifndef TRAITS_H_INCLUDED
#define TRAITS_H_INCLUDED

#include <type_traits>

namespace casmine
{
    namespace traits
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

        template <typename T>
        struct has_result_value
        {
            private:
                template <typename TTest>
                static typename TTest::result_value_type test(int);
                template <typename>
                static void test(...);
            public:
                enum { value = !::std::is_void<decltype(test<T>(0))>::value };
        };
    }
}

#endif // TRAITS_H_INCLUDED
