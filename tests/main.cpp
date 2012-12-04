#include <functional>
#include <iostream>
#include <vector>

#include "../source/assertions.h"
#include "../source/fixtures.h"

using namespace casmine;

fixture describe("more than one fixture in a module",
    test("test", []
		{
		})
	);

int main()
{
	return module::run();
}
