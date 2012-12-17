#ifndef PRINT_H_INCLUDED
#define PRINT_H_INCLUDED

#include <string>
#include <sstream>

#include "traits.h"

namespace casmine
{
    namespace to
    {
        template <typename T>
        ::std::string string(T value);
    }

    namespace print
    {

        template <typename T, typename Enable = void>
        struct string : ::std::string
        {
            string(T value)
            {
                ::std::stringstream string_value;
                string_value << value;
                this->assign(string_value.str());
            }
        };

        template <>
        struct string<bool, void> : ::std::string
        {
            string(bool value) { this->assign(value == true ? "true" : "false"); }
        };

        template <>
        struct string<char*, void> : ::std::string
        {
            string(char* value) { this->assign(value); }
        };

        template <typename T>
        struct string<T, typename ::std::enable_if<traits::has_const_iterator<T>::value>::type> : ::std::string
        {
            string(T value)
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

    namespace to
    {
        template <typename T>
        ::std::string string(T value) { return print::string<T>(value); }
    }
}

#endif // PRINT_H_INCLUDED
