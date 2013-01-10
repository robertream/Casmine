#include "../source/casmine.h"
#include "../source/sequence.h"
#include "constraints.assertion.failure.h"

using namespace std;

namespace assert_that
{
    fixture describe("assert::that tests",
        test("assertion message", []
        {
            auto action = []
            {
                "the message"
                    - CHECK_THAT(true, is::equal_to(false));
            };
            assert::throws(assertion::failure(__FILE__, 15, "the message", "The values are not equal.", "false", "true"), action);
        })
        +
        test("assertion message multiple assertions", []
        {
            auto action = []
            {
                "the message"
                    - (CHECK(true) == true)
                    - (CHECK(1) == 2);
            };
            assert::throws(assertion::failure(__FILE__, 26, "the message", "The values are not equal.", "2", "1"), action);
        })
    );
}
