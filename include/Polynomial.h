#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#include <iostream>
#include <vector>
#include <initializer_list>

class Polynomial
{
    public:
        Polynomial();
        Polynomial(int, std::vector<int>);
        void Scale(int);
        void AddRoot(int);
        void AddRoots(std::vector<int>);
        int EvaluatePolynomial(int);
        double ComputeDerivative(int);
        double ComputeIntegral(int, int);
        Polynomial operator+(const Polynomial&);
        Polynomial operator-(const Polynomial&);
        virtual ~Polynomial();
        int degree;
        std::vector<int> coefficients;
    protected:
    private:
};

#endif // POLYNOMIAL_H
