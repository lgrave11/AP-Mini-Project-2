#include <iostream>
#include <string>
#include <vector>
#include <algorithm>    // std::transform
#include <functional>   // std::plus
#include <complex>
#include "src/Polynomial.cpp" // Must include template implementation file.

int main()
{
    std::cout << "For double" << std::endl;
    PolynomialLib::Polynomial<double> bla_double {{5,3,-1,2}};
    PolynomialLib::Polynomial<double> bla2_double {{5,3,-1}};
    std::cout << bla_double << std::endl;
    std::cout << bla_double + bla2_double << std::endl;
    std::cout << bla_double * bla_double << std::endl;
    std::cout << bla_double.EvaluatePolynomial(5) << std::endl; // Should be 245
    std::cout << bla_double.ComputeDerivative(5) << std::endl; // Should be 143
    std::cout << bla_double.ComputeIntegral(0, 10) << std::endl; // Should be 4866.7

    std::cout << "For complex" << std::endl;

    // (5.0+1i) * x^0) + ((3.0+1i) * x^1) + ((-1.0+1i) * x^2) + ((2.0+1i) * x^3)
    std::complex<double> z1{5.0, 1.0};
    std::complex<double> z2{3.0, 1.0};
    std::complex<double> z3{-1.0, 1.0};
    std::complex<double> z4{2, 1.0};

    PolynomialLib::Polynomial<std::complex<double> > bla {{z1,z2,z3,z4}};
    std::cout << bla << std::endl;
    std::cout << bla + bla << std::endl;
    std::cout << bla * bla << std::endl;
    std::cout << bla.EvaluatePolynomial(5) << std::endl; // Should be 245
    std::cout << bla.ComputeDerivative(5) << std::endl; // Should be 143
    std::cout << bla.ComputeIntegral(0, 10) << std::endl; // Should be 4866.7
    return 0;
}
