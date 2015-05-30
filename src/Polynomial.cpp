#include "Polynomial.h"
#include <algorithm>
#include <math.h>

// a
Polynomial::Polynomial()
{
    //ctor
}

// b
// F.eks: [5,3,-1,2] = p(x) = (5 * x^0) + (3 * x^1) + (-1 * x^2) + (2 * x^3)
Polynomial::Polynomial(int deg, std::vector<int> coeffs)
{
    this->degree = deg;
    this->coefficients = coeffs;
}

// c
void Polynomial::Scale(int scalar)
{
    std::vector<int> scaledCoefficients;
    // Use const reference because we are not modifying i.
    // Also use auto&. Could also just use auto i.
    for(const auto& i : this->coefficients) {
        scaledCoefficients.push_back(i * scalar);
    }
    this->coefficients = scaledCoefficients;

}

// d
void Polynomial::AddRoot(int root)
{
    std::vector<int> addedRoot;
    auto lastValue = 0;
    addedRoot.push_back((coefficients[0] * root));
    lastValue = coefficients[0];

    for (auto i = 1; i < this->degree; i++) {
        addedRoot.push_back((lastValue - coefficients[i] * root));
        lastValue = coefficients[i];
    }


    /*for(const auto& i : this->coefficients) {
        if(index == 0) {
            addedRoot.push_back((-i * root));
        }
        else if(index == lastIndex) {
            addedRoot.push_back((i));
        }
        else {
            addedRoot.push_back((lastValue - i * root));
        }
        lastValue = i;
        index++;
    }*/
    this->coefficients = addedRoot;
}

// e
void Polynomial::AddRoots(std::vector<int> roots)
{
    for(const auto& i : roots) {
        this->AddRoot(i);
    }
}

// f
int Polynomial::EvaluatePolynomial(int x)
{
    auto result = 0;
    for (auto i = 0; i < this->degree; i++)
        result += this->coefficients[i] * pow(x, i);
    return result;
}

// g -- c1 + 2 * c2 * x + 3 * c3 * x^2 + ... + n * cn * x^n-1
double Polynomial::ComputeDerivative(int x)
{
    auto sum = 0.0f;
    auto p = 1.0f;
    auto counter = 1;

    for(int i=1; i < this->degree; i++)
    {
        sum += this->coefficients[i]*p*counter;
        counter++;
        p = p*x;
    }

    return sum;
}

// h, a metode to compute an integral for given interval bounds.
double Polynomial::ComputeIntegral(int a, int b)
{
    auto left = 0.0f;
    auto right = 0.0f;
    for (auto i=0; i < this->degree; i++)
    {
        std::cout << "((" << this->coefficients[i] << "/" << (i+1) << ") * " << b << "^" << (i+1) << ")" << std::endl;
        float tmp = ((float)this->coefficients[i] / (i+1)) * pow(b, i+1);
        left += tmp;
    }
    std::cout << std::endl;
    for (auto i=0; i < this->degree; i++)
    {
        std::cout << "((" << this->coefficients[i] << "/" << (i+1) << ") * " << a << "^" << (i+1) << ")" << std::endl;
        right += ((float)this->coefficients[i] / (i+1)) * pow(a, i+1);
    }
    std::cout << left << std::endl;
    std::cout << right << std::endl;

    return left - right;
}

Polynomial Polynomial::operator+(const Polynomial& rhs) {
    std::vector<int> result {};
    if(degree >= rhs.degree) {
        transform(coefficients.begin(),coefficients.end(),rhs.coefficients.begin(),std::back_inserter(result),std::plus<int>());
    }
    else {
        transform(rhs.coefficients.begin(),rhs.coefficients.end(),coefficients.begin(),std::back_inserter(result),std::plus<int>());
    }
    Polynomial res {result.size(), result};

    return res;
}

Polynomial Polynomial::operator-(const Polynomial& rhs) {
    std::vector<int> result {};
    if(degree >= rhs.degree) {
        transform(coefficients.begin(),coefficients.end(),rhs.coefficients.begin(),std::back_inserter(result),std::minus<int>());
    }
    else {
        transform(rhs.coefficients.begin(),rhs.coefficients.end(),coefficients.begin(),std::back_inserter(result),std::minus<int>());
    }

    Polynomial res {result.size(), result};

    return res;
}


Polynomial::~Polynomial()
{
    //dtor
}
