#include <iostream>
#include <string>
#include <vector>
#include <algorithm>    // std::transform
#include <functional>   // std::plus
#include "Polynomial.h"

int main()
{
    Polynomial bla {4, {5,3,-1,2}};
    Polynomial bla2 {4, {5,3,-1,2}};
    auto blah3 = bla + bla2;
    std::cout << blah3 << std::endl;
    return 0;
}
