#include <iostream>
#include <string>
#include <vector>
#include <algorithm>    // std::transform
#include <functional>   // std::plus
#include <complex>
#include <iterator>
#include <time.h>
#include "include/Polynomial.h" // Must include template implementation file.

int main()
{
    std::cout << "For int" << std::endl;
    std::array<int, 4> vec0{5,3, -1, 2};
    std::vector<int> vec1{5, 3, -1, 2};
    std::vector<int> vec2{5, 3, -1};
    Polynomial<int> bla0_int {};
    // Can also use template<Iterator> and std::begin(vec1), std::end(vec1)
    Polynomial<int> bla3_int {5,3,-1,2};
    Polynomial<int> bla1_int {vec0};
    Polynomial<int> bla_int {vec1};
    Polynomial<int> bla2_int {vec2};
    std::cout << "std::cout: " << bla_int << std::endl;
    // Scale
    //bla_int.Scale(5);
    //std::cout << bla_int << std::endl;
    // Add root
    //bla_int.AddRoot(5);
    //std::cout << "AddRoot(5): " << bla_int << std::endl;
    // Add roots
    //std::vector<int> vec3{5, 5, 5};
    //bla_int.AddRoots({5, 5, 5});
    //std::cout << "AddRoots({5,5,5}): " << bla_int << std::endl;
    std::cout << "Addition: ";
    std::cout << bla_int + bla2_int << std::endl;
    std::cout << "Addition: ";
    std::cout << bla2_int + bla_int << std::endl;
    std::cout << "Multiplication: ";
    std::cout << bla_int * bla_int << std::endl;
    std::cout << "EvaluatePolynomial(5): ";
    std::cout << bla_int.EvaluatePolynomial(5) << std::endl; // Should be 245
    std::cout << "ComputeDerivative(5): ";
    Polynomial<int> testBlah = bla_int.ComputeDerivative();
    std::cout << testBlah.EvaluatePolynomial(5) << std::endl; // Should be 143
    //std::cout << bla_int.ComputeIntegral(0, 10) << std::endl; // Should be 4866.7

    std::cout << "For double" << std::endl;
    std::vector<double> bla_vec{5, 3, -1, 2};
    std::vector<double> bla2_vec{5, 3, -1};
    Polynomial<double> bla_double {bla_vec};
    Polynomial<double> bla2_double {bla2_vec};
    std::cout << bla_double << std::endl;
    std::cout << bla_double + bla2_double << std::endl;
    std::cout << bla_double * bla_double << std::endl;
    std::cout << bla_double.EvaluatePolynomial(5) << std::endl; // Should be 245
    Polynomial<double> testBlah2 = bla_double.ComputeDerivative();
    std::cout << testBlah2.EvaluatePolynomial(5) << std::endl; // Should be 143
    std::cout << bla_double.ComputeIntegral(0, 10) << std::endl; // Should be 4866.7

    std::cout << "For complex" << std::endl;

    // (5.0+1i) * x^0) + ((3.0+1i) * x^1) + ((-1.0+1i) * x^2) + ((2.0+1i) * x^3)
    std::complex<double> z1{5.0, 1.0};
    std::complex<double> z2{3.0, 1.0};
    std::complex<double> z3{-1.0, 1.0};
    std::complex<double> z4{2, 1.0};

    std::vector<std::complex<double> > bla_vec_com{z1,z2,z3,z4};
    Polynomial<std::complex<double> > bla {bla_vec_com};
    std::cout << bla << std::endl;
    std::cout << bla + bla << std::endl;
    std::cout << bla * bla << std::endl;
    std::cout << bla.EvaluatePolynomial(5) << std::endl; // Should be 245
    Polynomial<std::complex<double> > testBlah3 = bla.ComputeDerivative();
    std::cout << testBlah3.EvaluatePolynomial(5) << std::endl; // Should be 143
    std::cout << bla.ComputeIntegral(0, 10) << std::endl; // Should be 4866.7
    return 0;
}
