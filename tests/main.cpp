#include <functional>
#include <iostream>
#include <vector>

#include "../source/assertions.h"
#include "../source/fixtures.h"

using namespace casmine;

fixture describe("simple fixture",
    test("test", []
		{
		})
	);

int main()
{
	return module::run();
}
