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
    }
}

#endif // TRAITS_H_INCLUDED
