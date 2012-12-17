#ifndef SEQUENCE_H_INCLUDED
#define SEQUENCE_H_INCLUDED

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
        ::std::list<T> result;
        result.push_back(value);
        return result;
    }

    template <typename T>
    ::std::list<T> sequence(T value1, T value2)
    {
        ::std::list<T> result;
        result.push_back(value1);
        result.push_back(value2);
        return result;
    }

    template <typename T>
    ::std::list<T> sequence(T value1, T value2, T value3)
    {
        ::std::list<T> result;
        result.push_back(value1);
        result.push_back(value2);
        result.push_back(value3);
        return result;
    }

    //template <typename T, typename ...TNext>
    //::std::list<T> sequence(T value, TNext... next)
    //{
    //    return ::std::list<T>({ value, next... });
    //}
}

#endif // SEQUENCE_H_INCLUDED
