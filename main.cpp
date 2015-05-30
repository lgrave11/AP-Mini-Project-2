#include <iostream>
#include <string>
#include <vector>
#include <algorithm>    // std::transform
#include <functional>   // std::plus
#include "Polynomial.h"

int main()
{
    Polynomial bla {4, {5,3,-1,2}};
    Polynomial bla2 {4, {2,3,4,5}};
    std::cout << bla.ComputeIntegral(0, 10) << std::endl;
    return 0;
}
