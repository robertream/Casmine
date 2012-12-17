#ifndef OUTPUT_H_INCLUDED
#define OUTPUT_H_INCLUDED

#include <string>
#include <iostream>

#include "assertion.h"

namespace casmine
{
    namespace output
    {
        struct console
        {
            ::std::string current_fixture;
            ::std::string current_test;

            void started(::std::string fixture, ::std::string test)
            {
                if (current_fixture != fixture)
                    ::std::cout << "* " << fixture << ::std::endl;
                ::std::cout << " - " << test;
                current_fixture = fixture;
                current_test = test;
            }

            void failed(::std::string fixture, ::std::string test, assertion::failure failure)
            {
                ::std::cout << " - failed." << ::std::endl;
                if (!failure.message.empty())
                {
                    ::std::cout << "  " + failure.message << ::std::endl;
                    ::std::cout << "   " + failure.description << ::std::endl;
                    ::std::cout << "    expected: " + failure.expected << ::std::endl;
                    ::std::cout << "    actual:   " + failure.actual   << ::std::endl;
                }
                else
                {
                    ::std::cout << "  " + failure.description << ::std::endl;
                    ::std::cout << "   expected: " + failure.expected << ::std::endl;
                    ::std::cout << "   actual:   " + failure.actual   << ::std::endl;

                }
            }

            //void failed(::std::string fixture, ::std::string test, ::std::exception error) { }

            void failed(::std::string fixture, ::std::string test)
            {
                ::std::cout << " - failed." << ::std::endl;
                ::std::cout << "  *unexpected exception*" << ::std::endl;
            }

            void succeded(::std::string fixture, ::std::string test)
            {
                ::std::cout << " - passed." << ::std::endl;
            }
        };
    }
}

#endif // OUTPUT_H_INCLUDED
