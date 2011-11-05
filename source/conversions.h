#ifndef CONVERSIONS_H_INCLUDED
#define CONVERSIONS_H_INCLUDED

#include <string>
#include <list>

namespace casmine
{
    template <typename T>
    ::std::list<T> sequence()
    {
        return ::std::list<T>();
    }

    template <typename T>
    ::std::list<T> sequence(T value)
    {
        return ::std::list<T>({ value });
    }

    template <typename T, typename ...TNext>
    ::std::list<T> sequence(T value, TNext... next)
    {
        return ::std::list<T>({ value, next... });
    }

    template <typename TContainer>
    ::std::list<typename TContainer::value_type> sequence_from(TContainer container)
    {
        ::std::list<typename TContainer::value_type> result;
        for (auto i = container.begin(); i != container.end(); i++)
            result.push_back(*i);
        return result;
    }
}

#endif // CONVERSIONS_H_INCLUDED
