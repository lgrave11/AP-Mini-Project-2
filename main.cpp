#include <iostream>
#include <string>
#include <vector>
#include <algorithm>    // std::transform
#include <functional>   // std::plus
#include <complex>
#include <iterator>
#include <chrono>
#include <ctime>
#include "include/Polynomial.h" // Must include template implementation file.

int main()
{
    std::cout << "For int" << std::endl;
    std::array<int, 4> vec0{5,3, -1, 2};
    std::vector<int> vec1{5, 3, -1, 2};
    std::vector<int> vec2{5, 3, -1};
    Polynomial<int> bla0_int {};
    Polynomial<int> bla3_int {5,3,-1,2};
    Polynomial<int> bla1_int {vec0};
    Polynomial<int> bla_int {vec1};
    Polynomial<int> bla2_int {vec2};
    std::cout << "std::cout: " << bla_int << std::endl;
    std::cout << "std::cout: " << bla2_int << std::endl;
    // Scale
    //bla_int.Scale(5);
    //std::cout << bla_int << std::endl;
    // Add root
    bla_int.AddRoot(5);
    std::cout << "AddRoot(5): " << bla_int << std::endl;
    // Add roots
    //bla_int.AddRoots({5,5,5});
    //std::cout << "AddRoots({5,5,5}): " << bla_int << std::endl;
    //bla_int.AddRoot(5);
    //std::cout << "AddRoot(5): " << bla_int << std::endl;
    //bla_int.AddRoot(5);
    //std::cout << "AddRoot(5): " << bla_int << std::endl;
    //bla_int.AddRoot(5);
    //std::cout << "AddRoot(5): " << bla_int << std::endl;

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
    std::cout << bla.ComputeIntegral(0, 10) << std::endl; // Should be 4866.7 blah

    /*/// Testing ideas:
    //(p1 + p2)(x) = p1(x) + p2(x);
    Polynomial<double> p1 {5,3};
    Polynomial<double> p2 {3,5};
    std::cout << "(p1 + p2)(x) = p1(x) + p2(x): ";
    std::cout << (p1 + p2).EvaluatePolynomial(5) << "==" << p1.EvaluatePolynomial(5) + p2.EvaluatePolynomial(5) << std::endl;
    std::cout << "(p1 * p2)(x) = p1(x) * p2(x): ";
    std::cout << (p1 * p2).EvaluatePolynomial(5) << "==" << p1.EvaluatePolynomial(5) * p2.EvaluatePolynomial(5) << std::endl;
    std::cout << "if p(x) = (x-r1) * (x-r2), then p'((r1 + r2) / 2) == 0: ";
    double r1 = 5;
    double r2 = 3;
    Polynomial<double> p {r1 * r2, (-r1-r2), 1};
    std::cout << p.ComputeDerivative().EvaluatePolynomial((r1+r2)/2) << std::endl;

    Polynomial<double> a{ 2 };
    Polynomial<double> myPoly{ 1, 2, 1 };
    myPoly.AddRoot(0);
    myPoly = a * myPoly;
    myPoly = myPoly * myPoly;
    myPoly = myPoly * myPoly;
    std::cout << myPoly << std::endl;*/

    /*Polynomial<double> test{1,2,3};
    Polynomial<double> test2{3,2,1};
    test = std::move(test2);
    std::cout << test << std::endl;*/

    /*std::vector<double> blah;
    for(double i = 0; i < 1000000; i++) {
        blah.push_back(i);
    }

    Polynomial<double> test{blah};
    auto start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    test.ComputeIntegral(0,10); // Cache
    auto mid_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    std::cout << (mid_time - start_time).count() << std::endl;
    test.ComputeIntegral(0,10);
    auto end_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

    auto time_taken = (end_time - mid_time).count();
    std::cout << time_taken << std::endl;*/


    return 0;
}
