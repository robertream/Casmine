#ifndef CONVERSIONS_H_INCLUDED
#define CONVERSIONS_H_INCLUDED

#include <string>
#include <vector>

namespace casmine
{
    template <typename T>
    ::std::vector<T> sequence()
    {
        return ::std::vector<T>();
    }

    template <typename T>
    ::std::vector<T> sequence(T value)
    {
        return ::std::vector<T>({ value });
    }

    template <typename T, typename ...TNext>
    ::std::vector<T> sequence(T value, TNext... next)
    {
        return ::std::vector<T>({ value, next... });
    }

    template <typename TContainer>
    ::std::vector<typename TContainer::value_type> sequence_from(TContainer container)
    {
        ::std::vector<typename TContainer::value_type> result;
        for (auto i = container.begin(); i != container.end(); i++)
            result.push_back(*i);
        return result;
    }
}

#endif // CONVERSIONS_H_INCLUDED
