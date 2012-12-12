#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
#include <numeric>

namespace casmine
{
    struct fixture;

    struct module
    {
        ::std::vector<fixture*> fixtures;
        static void add(fixture* f) { instance().fixtures.push_back(f); }
        static int run(int argc, char *argv[])
        {
            int errors = instance().run_all();
            if (argc > 1)
                std::cin.get();
            return errors;
        }

        private:
            module() { }
            static module& instance() { static module instance; return instance; }
            int run_all();
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

        int run()
        {
            ::std::cout << "* " << description << ::std::endl;
            return ::std::accumulate(tests.begin(), tests.end(), 0, [] (int failures, test current_test) -> int
                {
                    try
                    {
                        ::std::cout << " - " << current_test.description;
                        current_test.run_test();
                        ::std::cout << " - passed." << ::std::endl;
                        return failures;
                    }
                    catch(assertion::failure failure)
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
                        return failures + 1;
                    }
                    catch(...)
                    {
                        ::std::cout << " - failed." << ::std::endl;
                        ::std::cout << "  *unexpected exception*" << ::std::endl;
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

    inline int module::run_all()
    {
        auto& fixtures = module::instance().fixtures;
        return ::std::accumulate(fixtures.begin(), fixtures.end(), 0, [] (int failures, fixture* current_fixture) -> int
            {
                return failures + current_fixture->run();
            });
    }
}
