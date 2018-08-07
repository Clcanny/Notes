#include <iostream>
#include "include/valgrind.h"

int main()
{
    std::cout <<"running on valgrind: " << RUNNING_ON_VALGRIND << std::endl;

    VALGRIND_PRINTF("%s\n", "valgrind printf");
    VALGRIND_PRINTF_BACKTRACE("%s\n", "valgrind printf backtrace");
}
