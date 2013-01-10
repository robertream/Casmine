#ifndef TYPENAME_H_INCLUDED
#define TYPENAME_H_INCLUDED

#include <typeinfo>

#ifdef __GNUC__
    #include <cxxabi.h>
#endif

inline ::std::string typename_of(const ::std::type_info& type)
{
    #ifdef _GLIBCXX_CDTOR_CALLABI
        ::std::string name;
        int status;
        char *realname = abi::__cxa_demangle(type.name(), 0, 0, &status);
        name = realname;
        free(realname);
        return name;
    #else
        return type.name();
    #endif
}

template <typename TType>
::std::string typename_of(const TType& value)
{
    return typename_of(typeid(value));
}

template <typename TType>
::std::string typename_of()
{
    return typename_of(typeid(TType));
}

#endif // TYPENAME_H_INCLUDED
