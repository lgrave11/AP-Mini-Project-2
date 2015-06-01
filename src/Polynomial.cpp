#include "Polynomial.h"
#include <algorithm>
#include <math.h>
#include <type_traits>
#include <iterator>

namespace PolynomialLib {
    // a
    template<typename T>
    Polynomial<T>::Polynomial()
    {
    }

    // b
    // F.eks: [5,3,-1,2] = p(x) = (5 * x^0) + (3 * x^1) + (-1 * x^2) + (2 * x^3)
    template<typename T>
    //template<typename C>
    Polynomial<T>::Polynomial(std::vector<T> coeffs)
    {
        this->degree = coeffs.size();
        this->coefficients = coeffs;
    }

    template<typename T>
    Polynomial<T>::~Polynomial()
    {
    }

    // c
    template<typename T>
    void Polynomial<T>::Scale(const T scalar)
    {
        std::vector<T> scaledCoefficients;
        // Use const reference because we are not modifying i.
        // Also use auto&. Could also just use auto i.
        for(const auto& i : this->coefficients) {
            auto result = i * scalar;
            scaledCoefficients.push_back(result);
        }
        //std::transform(std::begin(this->coefficients), std::end(this->coefficients), std::begin(scaledCoefficients), std::bind1st(std::multiplies<T>(), scalar));
        this->coefficients = scaledCoefficients;

    }

    // d
    template<typename T>
    void Polynomial<T>::AddRoot(const T root)
    {
        std::vector<T> addedRoot {};
        T lastValue{};
        addedRoot.push_back((-this->coefficients[0] * root));
        lastValue = this->coefficients[0];

        for (auto i = 1; i < this->degree; i++) {
            addedRoot.push_back((lastValue - this->coefficients[i] * root));
            lastValue = this->coefficients[i];
        }
        addedRoot.push_back(lastValue);
        this->coefficients = addedRoot;
        this->degree = this->coefficients.size();
    }

    // e
    template<typename T>
    //template<typename C>
    void Polynomial<T>::AddRoots(const std::vector<T> roots)
    {
        for(const auto& i : roots) {
            this->AddRoot(i);
        }
    }

    // f
    template<typename T>
    T Polynomial<T>::EvaluatePolynomial(const T x)
    {
        T result{};
        for (auto i = 0; i < this->degree; i++)
            result = result + (this->coefficients[i] * pow(x, i));
        return result;
    }

    // g -- (1*c1 * x^0) + (2*c2 * x^1) + (3*c3 * x^2) + ... + n * cn * x^n-1
    template<typename T>
    T Polynomial<T>::ComputeDerivative(const T x)
    {
        T sum{};
        double p = 1;
        int counter = 0;
        double i = 0;
        for(const auto& c : coefficients) {
            if(counter++ == 0) {
                continue;
            }
            sum += p * c * pow(x, i);
            i++;
            p++;
        }


        return sum;
    }

    // h, a method to compute an integral for given interval bounds.
    template<typename T>
    T Polynomial<T>::ComputeIntegral(const T a, const T b)
    {
        // Alternatively I could have used enable_if here (std::enable_if_t<!std::is_integral<T>::value, T>),
        // but I like the static_assert message more.
        static_assert(!std::is_integral<T>::value,"ComputeIntegral is not supported for int types.");
        T left {};
        T right {};
        for (auto i=0.0; i < this->degree; i++)
        {
            //std::cout << "((" << this->coefficients[i] << "/" << (i+1) << ") * " << b << "^" << (i+1) << ")" << std::endl;
            left += (this->coefficients[i] / (i+1)) * pow(b, i+1);
        }
        //std::cout << std::endl;
        for (auto i=0.0; i < this->degree; i++)
        {
            //std::cout << "((" << this->coefficients[i] << "/" << (i+1) << ") * " << a << "^" << (i+1) << ")" << std::endl;
            right += (this->coefficients[i] / (i+1)) * pow(a, i+1);
        }

        return left - right;
    }

    template<typename T>
    Polynomial<T> Polynomial<T>::operator+(const Polynomial<T>& rhs) {
        std::vector<T> result {};
        if(degree >= rhs.degree) {
            transform(coefficients.begin(),coefficients.end(),rhs.coefficients.begin(),std::back_inserter(result),std::plus<T>());
        }
        else {
            transform(rhs.coefficients.begin(),rhs.coefficients.end(),coefficients.begin(),std::back_inserter(result),std::plus<T>());
        }
        Polynomial<T> res {result};

        return res;
    }

    template<typename T>
    Polynomial<T> Polynomial<T>::operator*(const Polynomial<T>& rhs) {
        // Initialize vector.
        const int newDegree = degree + rhs.degree-1;
        std::vector<T> tmp(newDegree);

        int iCount = 0;
        for(auto i : coefficients) {
            int jCount = 0;
            for(const auto& j : rhs.coefficients) {
                tmp[iCount+jCount] += i * j;
                jCount++;
            }
            iCount++;
        }

        Polynomial<T> res {tmp};

        return res;
    }

    template<typename T>
    std::ostream& operator<< (std::ostream& out, const Polynomial<T>& pol) {
        int index = 0;
        int lastIndex = pol.coefficients.size() - 1;
        for(const auto& coeff : pol.coefficients)
        {
            out << "(" << coeff << "*" << "x^" << index << ")";
            if(index != lastIndex) {
                out << " + ";
            }
            index++;
        }
        return out;
    }

}
