#include "../source/casmine.h"

fixture describe("simple fixture",
    test("test", []
        {
        })
    );

int main(int argc, char *argv[])
{
    return module::run<output::console>(argc, argv);
}
