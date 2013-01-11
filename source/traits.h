#ifndef TRAITS_H_INCLUDED
#define TRAITS_H_INCLUDED

#include <type_traits>

namespace casmine
{
    namespace traits
    {
        template <typename TType>
        struct has_const_iterator
        {
            private:
                template <typename TTest>
                static typename TTest::const_iterator test(int);
                template <typename>
                static void test(...);
            public:
                enum { value = !::std::is_void<decltype(test<TType>(0))>::value };
        };

        template <typename TType>
        struct has_result_value
        {
            private:
                template <typename TTest>
                static typename TTest::result_type test(int);
                template <typename>
                static void test(...);
            public:
                enum { value = !::std::is_void<decltype(test<TType>(0))>::value };
        };

        template <typename TType>
        struct type_of
        {
            typedef TType type;
        };

        template <typename TType>
        struct result_of
        {
            typedef typename TType::result_type type;
        };
    }
}

#endif // TRAITS_H_INCLUDED
