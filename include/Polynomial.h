#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#include <iostream>
#include <vector>
#include <iostream>
#include <initializer_list>
#include <unordered_map>
#include <mutex>
#include <sstream>
#include <memory>
#include <future>
#include <numeric>
#include <cmath>

/**
    Name: Lasse Vang Gravesen
    Email: lgrave11@student.aau.dk
    Study: Software 8

    Requirements fulfilled to the best of my knowledge:
    1a-j, 2, 3, 4, 5, 6, 7, 8, 9, 10

*/

// 2. Make a template class of Polynomial for a given type of coefficients .
template<typename T>
class Polynomial
{
    public:
        std::unique_ptr<std::vector<T> > coefficients;

        // 1.(a) Default constructor to create a trivial polynomial: 0.
        Polynomial() : coefficients { std::make_unique<std::vector<T> >(0) } { }
        virtual ~Polynomial() {}
        Polynomial(const Polynomial<T>& oPoly) = delete; // Copy constructor
        Polynomial& operator= (const Polynomial<T>& oPoly) = delete; // Copy assignment operator.
        // 7. Implement move semantics using smart pointers.
        // Use move assignment operator in the move constructor.
        Polynomial(Polynomial<T>&& oPoly) {
            *this = std::move(oPoly);
        }// Move constructor

        Polynomial& operator= (Polynomial<T>&& oPoly) {
            if (this != &oPoly)
            {
                this->coefficients = std::move(oPoly.coefficients);
                this->cacheValid = false;
            }
            return *this;
        }; // Move assignment operator.

        // 1.(b) Constructor for specific degree term coefficients.
        // 5. Member functions accepting containers should support any type of container, including array types
        // 4. Use const where applicable
        template<typename Container>
        Polynomial(const Container& c) : coefficients{ std::make_unique<std::vector<T> >() } {
            // 3. Use auto where applicable
            for(auto it = std::cbegin(c); it != std::cend(c); it++) {
                this->coefficients->emplace_back(*it);
            }
        }

        // Support brace initialization with a regular constructor.
        Polynomial(const std::initializer_list<T> c) : coefficients{ std::make_unique<std::vector<T> >(c) } {}

        // 1.(c) A method to scale the polynomial, i.e. multiply by a scalar value.
        void Scale(const T scalar) {
            std::lock_guard<std::mutex> g(m);
            std::unique_ptr<std::vector<T> > scaledCoefficients = std::make_unique<std::vector<T> >();
            /// Use std::transform with a lambda instead of a traditional loop.
            // 9. Use lambda expressions.
            std::transform(std::begin((*this->coefficients)), std::end((*this->coefficients)), std::back_inserter(*scaledCoefficients), [&scalar](const T& i) {return i * scalar;});
            this->coefficients = std::move(scaledCoefficients);

            this->cacheValid = false;
        }

        // 1.(d) A method to add a root r, i.e. multiply by a term (x-r).
        void AddRoot(const T root) {
            std::lock_guard<std::mutex> g(m);
            std::unique_ptr<std::vector<T> > addedRoot = std::make_unique<std::vector<T> >();
            T lastValue{};
            (*addedRoot).push_back((-(*this->coefficients)[0] * root));
            lastValue = (*this->coefficients)[0];

            for (auto i = 1.0; i < (*this->coefficients).size(); i++) {
                (*addedRoot).push_back((lastValue - (*this->coefficients)[i] * root));
                lastValue = (*this->coefficients)[i];
            }
            (*addedRoot).push_back(lastValue);
            this->coefficients = std::move(addedRoot);

            this->cacheValid = false;
        }

        // 1.(e) A method to add several roots at once.
        // 5. Member functions accepting containers should support any type of container, including array types
        template<typename Container = std::initializer_list<T>>
        void AddRoots(const Container& c)
        {
            for(auto it = std::begin(c); it != std::end(c); it++) {
                this->AddRoot(*it);
            }
        }

        // 1.(f) A method to valuate the polynomial at a given point.
        T EvaluatePolynomial(const T x) const
        {
            /// Use lambda expressions (Chapter 6, e.g. when computing sums during evaluation of a polynomial; dispatch asynchronous computation).
            T result{};

            auto i = 0;

            // Use non-member begin and end for making it more generic.
            std::for_each(std::begin((*this->coefficients)),std::end((*this->coefficients)),[&result, &i, x](T n){ result += (n * pow(x, i)); i++; });

            /// Alternatively I could have used an ordinary for loop:
            //for (auto i = 0; i < this->coefficients.size(); i++)
            //    result += (this->coefficients[i] * pow(x, i));
            return result;
        }

        // 1.(g) A method to compute a polynomial which is a derivative of the polynomial.
        // (1 * c1 * x^0) + (2 * c2 * x^1) + (3 * c3 * x^2) + ... + (n * cn * x^(n-1))
        Polynomial<T> ComputeDerivative() const
        {
            Polynomial<T> ret {};
            ret.coefficients->clear();
            double p = 1;
            int counter = 0;
            double i = 0;
            for(const auto& c : (*this->coefficients)) {
                if(counter++ == 0) {
                    continue;
                }
                ret.coefficients->push_back(p * c);
                i++;
                p++;
            }
            return ret;
        }

        // 1.(h) A method to compute an integral for given interval bounds.
        T ComputeIntegral(const T a, const T b) const {
            // 8. Use type traits
            // Alternatively I could have used enable_if here (std::enable_if_t<!std::is_integral<T>::value, T>).
            static_assert(!std::is_integral<T>::value,"ComputeIntegral is not supported for integer types.");
            integralCache();
            // 10. Use concurrency, might not make sense from a design perspective, but I struggled with here to put it.
            auto bFuture = std::async(std::launch::async,[&](T val) { return this->integral->EvaluatePolynomial(val); }, b);
            auto aFuture = std::async(std::launch::async, [&](T val) { return this->integral->EvaluatePolynomial(val); }, a);
            return bFuture.get() - aFuture.get();
            //return this->integral->EvaluatePolynomial(b) - this->integral->EvaluatePolynomial(a);
        }

        // 6. Cache the integral data to avoid repetitive integration (Items 16, 40).
        void integralCache() const {
            // Make it thread-safe.
            std::lock_guard<std::mutex> g(m);
            if(this->cacheValid == false) {
                std::vector<T> results;
                results.push_back(0);
                for(auto i = 0.0; i < this->coefficients->size(); i++) {
                    results.push_back((*this->coefficients)[i] / (i+1));
                }
                this->integral = std::make_unique<Polynomial<T> >(results); //std::unique_ptr<Polynomial>{ new Polynomial{results} };
            }
            this->cacheValid = true;
        }

        // 1.(i) A plus operator to return a polynomial equal to a sum of two polynomials.
        Polynomial<T> operator+(const Polynomial<T>& rhs) const {
            std::vector<T> tmp;
            std::vector<T> result;

            if(this->coefficients->size() >= rhs.coefficients->size()) {
                tmp = (*rhs.coefficients);
                tmp.insert(tmp.end(),this->coefficients->size() - rhs.coefficients->size(), T{});
                // Could also use std::plus<T> here.
                transform(std::begin((*this->coefficients)),std::end((*this->coefficients)),std::begin(tmp),std::back_inserter(result),[](T l, T r) { return l + r; });
            }
            else {
                tmp = (*this->coefficients);
                tmp.insert(tmp.end(), rhs.coefficients->size() - this->coefficients->size(), T{});
                transform(std::begin((*rhs.coefficients)),std::end((*rhs.coefficients)),std::begin(tmp),std::back_inserter(result),[](T l, T r) { return l + r; });
            }

            Polynomial<T> res {result};

            return res;
        }

        // 1.(j) A star operator to return a polynomial equal to a product of two polynomials.
        Polynomial<T> operator*(const Polynomial<T>& rhs) const {
            // Initialize vector.
            const auto newDegree = this->coefficients->size() + rhs.coefficients->size()-1;
            std::vector<T> tmp(newDegree);

            int iCount = 0;
            for(const auto& i : (*this->coefficients)) {
                int jCount = 0;
                for(const auto& j : (*rhs.coefficients)) {
                    tmp[iCount+jCount] += i * j;
                    jCount++;
                }
                iCount++;
            }

            Polynomial<T> res {tmp};

            return res;
        }

        // Also adding a operator<< to make it easier to debug and use the library.
        friend std::ostream& operator<<(std::ostream& out, const Polynomial<T>& pol) {
            int index = 0;
            int lastIndex = pol.coefficients->size() - 1;
            for(const auto& i : (*pol.coefficients))
            {
                out << "(" << i << "*" << "x^" << index << ")";
                if(index != lastIndex) {
                    out << " + ";
                }
                index++;
            }
            return out;
        }
    protected:
    private:
        mutable std::mutex m;
        mutable bool cacheValid = false;
        mutable std::unique_ptr<Polynomial<T> > integral {};
};



#endif // POLYNOMIAL_H
