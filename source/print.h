#ifndef PRINT_H_INCLUDED
#define PRINT_H_INCLUDED

#include <string>
#include <list>
#include <sstream>

namespace casmine
{
    namespace print
    {
        template <typename T, bool CanIterate = traits::has_const_iterator<T>::value>
        struct string;
    }

    namespace to
    {
        template <typename T>
        ::std::string string(const T& value) { return print::string<T>(value); }
    }

    namespace print
    {
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
}

#endif // PRINT_H_INCLUDED
