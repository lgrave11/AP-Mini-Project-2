#include "Polynomial.h"
#include <algorithm>
#include <math.h>

namespace PolynomialLib {
    // a
    template<typename C>
    Polynomial<C>::Polynomial()
    {
        //ctor
    }

    // b
    // F.eks: [5,3,-1,2] = p(x) = (5 * x^0) + (3 * x^1) + (-1 * x^2) + (2 * x^3)
    template<typename C>
    Polynomial<C>::Polynomial(int deg, std::vector<C> coeffs)
    {
        this->degree = deg;
        this->coefficients = coeffs;
    }

    // c
    template<typename C>
    void Polynomial<C>::Scale(C scalar)
    {
        std::vector<C> scaledCoefficients;
        // Use const reference because we are not modifying i.
        // Also use auto&. Could also just use auto i.
        for(const auto& i : this->coefficients) {
            scaledCoefficients.push_back(i * scalar);
        }
        this->coefficients = scaledCoefficients;

    }

    // d
    template<typename C>
    void Polynomial<C>::AddRoot(C root)
    {
        std::vector<C> addedRoot;
        auto lastValue = 0;
        addedRoot.push_back((coefficients[0] * root));
        lastValue = coefficients[0];

        for (auto i = 1; i < this->degree; i++) {
            addedRoot.push_back((lastValue - coefficients[i] * root));
            lastValue = coefficients[i];
        }
        this->coefficients = addedRoot;
    }

    // e
    template<typename C>
    void Polynomial<C>::AddRoots(std::vector<C> roots)
    {
        for(const auto& i : roots) {
            this->AddRoot(i);
        }
    }

    // f
    template<typename C>
    C Polynomial<C>::EvaluatePolynomial(C x)
    {
        auto result = 0;
        for (auto i = 0; i < this->degree; i++)
            result += this->coefficients[i] * pow(x, i);
        return result;
    }

    // g -- c1 + 2 * c2 * x + 3 * c3 * x^2 + ... + n * cn * x^n-1
    template<typename C>
    C Polynomial<C>::ComputeDerivative(C x)
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

    // h, a method to compute an integral for given interval bounds.
    template<typename C>
    C Polynomial<C>::ComputeIntegral(C a, C b)
    {
        auto left = 0.0f;
        auto right = 0.0f;
        for (auto i=0; i < this->degree; i++)
        {
            //std::cout << "((" << this->coefficients[i] << "/" << (i+1) << ") * " << b << "^" << (i+1) << ")" << std::endl;
            float tmp = ((float)this->coefficients[i] / (i+1)) * pow(b, i+1);
            left += tmp;
        }
        //std::cout << std::endl;
        for (auto i=0; i < this->degree; i++)
        {
            //std::cout << "((" << this->coefficients[i] << "/" << (i+1) << ") * " << a << "^" << (i+1) << ")" << std::endl;
            right += ((float)this->coefficients[i] / (i+1)) * pow(a, i+1);
        }
        //std::cout << left << std::endl;
        //std::cout << right << std::endl;

        return left - right;
    }

    template<typename C>
    Polynomial<C> Polynomial<C>::operator+(const Polynomial<C>& rhs) {
        std::vector<C> result {};
        if(degree >= rhs.degree) {
            transform(coefficients.begin(),coefficients.end(),rhs.coefficients.begin(),std::back_inserter(result),std::plus<C>());
        }
        else {
            transform(rhs.coefficients.begin(),rhs.coefficients.end(),coefficients.begin(),std::back_inserter(result),std::plus<C>());
        }
        Polynomial<C> res {(int)result.size(), result};  // Cast to int or it gives narrowing warning from int to unsigned int.

        return res;
    }

    template<typename C>
    Polynomial<C> Polynomial<C>::operator-(const Polynomial<C>& rhs) {
        std::vector<C> result {};
        if(degree >= rhs.degree) {
            transform(coefficients.begin(),coefficients.end(),rhs.coefficients.begin(),std::back_inserter(result),std::minus<C>());
        }
        else {
            transform(rhs.coefficients.begin(),rhs.coefficients.end(),coefficients.begin(),std::back_inserter(result),std::minus<C>());
        }

        Polynomial<C> res {(int)result.size(), result}; // Cast to int or it gives narrowing warning from int to unsigned int.

        return res;
    }

    template<typename C>
    std::ostream& operator<< (std::ostream& out, const Polynomial<C>& pol) {
        int index = 0;
        int lastIndex = pol.coefficients.size() - 1;
        for(auto coeff : pol.coefficients)
        {
            out << "(" << coeff << "*" << "x^" << index << ")";
            if(index != lastIndex) {
                out << " + ";
            }
            index++;
        }
        return out;
    }

    template<typename C>
    Polynomial<C>::~Polynomial()
    {
        //dtor
    }

}