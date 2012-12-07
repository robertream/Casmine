#include "../source/casmine.h"

fixture describe("simple fixture",
    test("test", []
        {
        })
    );

int main()
{
    return module::run();
}
