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

            void started(const ::std::string& fixture, const ::std::string& test)
            {
                if (current_fixture != fixture)
                    ::std::cout << "* " << fixture << ::std::endl;
                ::std::cout << " - " << test;
                current_fixture = fixture;
                current_test = test;
            }

            void failed(const ::std::string& fixture, const ::std::string& test, const assertion::failure& failure)
            {
                ::std::cout << " - failed." << ::std::endl;
                if (!failure.message.empty())
                {
                    if (!failure.file.empty())
                        ::std::cout << failure.file << "(" << failure.line << "): " << failure.message << ::std::endl;
                    else
                        ::std::cout << "  " + failure.message << ::std::endl;
                    ::std::cout << "   " + failure.description << ::std::endl;
                    ::std::cout << "    expected: " << failure.expected << ::std::endl;
                    ::std::cout << "    actual:   " << failure.actual   << ::std::endl;
                }
                else
                {
                    if (!failure.file.empty())
                        ::std::cout << failure.file << "(" << failure.line << "): " << failure.description << ::std::endl;
                    else
                        ::std::cout << "  " + failure.description << ::std::endl;
                    ::std::cout << "   expected: " << failure.expected << ::std::endl;
                    ::std::cout << "   actual:   " << failure.actual   << ::std::endl;
                }
            }

            void failed(const ::std::string& fixture, const ::std::string& test, const ::std::exception& error)
            {
                ::std::cout << " - failed." << ::std::endl;
                ::std::cout << "  unexpected exception: " << typename_of(error) << ::std::endl;
                if (error.what() != nullptr)
                {
                    ::std::string what(error.what());
                    if (!what.empty())
                        ::std::cout << "   " << what << ::std::endl;
                }
            }

            void failed(const ::std::string& fixture, const ::std::string& test)
            {
                ::std::cout << " - failed." << ::std::endl;
                ::std::cout << "  unexpected exception" << ::std::endl;
            }

            void succeded(const ::std::string& fixture, const ::std::string& test)
            {
                ::std::cout << " - passed." << ::std::endl;
            }
        };
    }
}

#endif // OUTPUT_H_INCLUDED
