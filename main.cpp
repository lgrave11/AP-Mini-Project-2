#include <iostream>
#include <string>
#include <vector>
#include <algorithm>    // std::transform
#include <functional>   // std::plus
#include <complex>
#include "src/Polynomial.cpp" // Must include template implementation file.

int main()
{
    PolynomialLib::Polynomial<int> bla {{5,3,-1,2}};
    PolynomialLib::Polynomial<int> bla2 {{5,3,-1}};
    std::cout << bla << std::endl;
    std::cout << bla2 << std::endl;
    std::cout << bla + bla2 << std::endl;
    std::cout << bla2 * bla << std::endl;
    /*std::cout << bla.EvaluatePolynomial(5) << std::endl; // Should be 245
    std::cout << bla.ComputeDerivative(5) << std::endl; // Should be 143
    std::cout << bla.ComputeIntegral(0, 10) << std::endl; // Should be 4866.7*/

    /*std::complex<double> z1{0.0, 1.0};

    std::complex<double> z2{5.0, 1.0};
    std::complex<double> z3{10.0, 1.0};
    std::complex<double> z4{15.0, 1.0};

    PolynomialLib::Polynomial<std::complex<double> > bla {4, {z1,z2,z3,z4}};
    std::cout << bla << std::endl;
    std::cout << bla.EvaluatePolynomial(5) << std::endl; // Should be 245
    std::cout << bla.ComputeDerivative(5) << std::endl; // Should be 143
    std::cout << bla.ComputeIntegral(0, 10) << std::endl; // Should be 4866.7*/
    return 0;
}
