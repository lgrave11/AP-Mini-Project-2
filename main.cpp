#include <iostream>
#include <string>
#include <vector>
#include <algorithm>    // std::transform
#include <functional>   // std::plus
#include <complex>
#include "src/Polynomial.cpp" // Must include template implementation file.

int main()
{
    PolynomialLib::Polynomial<double> bla {4, {5,3,-1,2}};
    PolynomialLib::Polynomial<double> bla2 {4, {5.5,3.3,-1.1,2.2}};
    std::cout << bla << std::endl;
    std::cout << bla + bla2 << std::endl;
    std::cout << bla - bla2 << std::endl;
    std::cout << bla.EvaluatePolynomial(5) << std::endl; // Should be 245
    std::cout << bla.ComputeDerivative(5) << std::endl; // Should be 143
    std::cout << bla.ComputeIntegral(0, 10) << std::endl; // Should be 4866.7
    return 0;
}