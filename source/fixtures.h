#ifndef FIXTURES_H_INCLUDED
#define FIXTURES_H_INCLUDED

#include <algorithm>
#include <functional>
#include <vector>
#include <numeric>

#include "assertion.h"

namespace casmine
{
    struct fixture;

    struct module
    {
        ::std::vector<fixture*> fixtures;
        static void add(fixture* f) { instance().fixtures.push_back(f); }
        
        template<typename TOutput>
        static int run(int argc, char *argv[], TOutput output = TOutput())
        {
            int errors = instance().run_all(output);
            if (argc > 1)
                std::cin.get();
            return errors;
        }

    private:
            module() { }
            static module& instance() { static module instance; return instance; }
            
            template<typename TOutput>
            int run_all(TOutput output);
    };

    struct test
    {
        test(::std::string description, ::std::function<void (void)> run_test)
            : description(description), run_test(run_test) { }

        ::std::string description;
        ::std::function<void (void)> run_test;
    };

    struct fixture
    {
        ::std::string description;
        ::std::vector<test> tests;

        fixture(const ::std::string description, test first_test)
            : description(description)
        {
            tests.push_back(first_test);
            module::add(this);
        }

        fixture(const ::std::string description, ::std::vector<test> tests)
            : description(description), tests(tests)
        {
            module::add(this);
        }

        template<typename TOutput>
        int run(TOutput output)
        {
            return ::std::accumulate(tests.begin(), tests.end(), 0, [&] (int failures, test current_test) -> int
                {
                    try
                    {
                        output.started(description, current_test.description);
                        current_test.run_test();
                        output.succeded(description, current_test.description);
                        return failures;
                    }
                    catch(assertion::failure failure)
                    {
                        output.failed(description, current_test.description, failure);
                        return failures + 1;
                    }
                    catch(...)
                    {
                        output.failed(description, current_test.description);
                        return failures + 1;
                    }
                });
        }
    };

    inline ::std::vector<test> operator +(test first_test, test second_test)
    {
        ::std::vector<test> tests;
        tests.push_back(first_test);
        tests.push_back(second_test);
        return tests;
    }

    inline ::std::vector<test> operator +(::std::vector<test> tests, test test_to_add)
    {
        tests.push_back(test_to_add);
        return tests;
    }
    
    template<typename TOutput>
    inline int module::run_all(TOutput output)
    {
        auto& fixtures = module::instance().fixtures;
        return ::std::accumulate(fixtures.begin(), fixtures.end(), 0, [&] (int failures, fixture* current_fixture) -> int
            {
                return failures + current_fixture->run(output);
            });
    }
}

#endif // FIXTURES_H_INCLUDED
