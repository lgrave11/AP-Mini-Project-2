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

//2. Make a template class of Polynomial for a given type of coefficients...
template<typename T>
class Polynomial
{
    public:
        std::vector<T> coefficients;

        // (a) Default constructor to create a trivial polynomial: 0.
        Polynomial() : coefficients { std::vector<T> {0} } { }
        virtual ~Polynomial() {}
        Polynomial(const Polynomial<T>& oPoly) = delete; // Copy constructor
        Polynomial& operator= (const Polynomial<T>& oPoly) = delete; // Copy assignment operator.
        Polynomial(Polynomial<T>&& oPoly) {
            *this = std::move(oPoly);
        }// Move constructor
        Polynomial& operator= (Polynomial<T>&& oPoly) {
            this->coefficients = oPoly.coefficients;
            return *this;
        }; // Move assignment operator.

        // (b) Constructor for specific degree term coefficients.
        // 5. Member functions accepting containers should support any type of container, including array types
        // 4. Use const where applicable
        template<typename Container>
        Polynomial(const Container& c) : coefficients { std::vector<T> {} } {
            // 3. Use auto where applicable
            for(auto it = std::cbegin(c); it != std::cend(c); it++) {
                this->coefficients.emplace_back(*it);
            }
        }

        // Support brace initialization with a regular constructor.
        Polynomial(const std::initializer_list<T> c) : coefficients{ std::vector<T> {c} } {}

        // (c) A method to scale the polynomial, i.e. multiply by a scalar value.
        void Scale(const T scalar) {
            //std::lock_guard<std::mutex> g(m);
            std::vector<T> scaledCoefficients;
            /// Use std::transform with a lambda instead of a traditional loop.
            // Requirement 9: Use lambda expressions.
            std::transform(std::begin(this->coefficients), std::end(this->coefficients), std::back_inserter(scaledCoefficients), [&scalar](const T& i) {return i * scalar;});
            this->coefficients = scaledCoefficients;

            this->cacheValid = false;
        }

        // (d) A method to add a root r, i.e. multiply by a term (x-r).
        void AddRoot(const T root) {
            //std::lock_guard<std::mutex> g(m);
            std::vector<T> addedRoot {};
            T lastValue{};
            addedRoot.push_back((-this->coefficients[0] * root));
            lastValue = this->coefficients[0];

            for (auto i = 1.0; i < this->coefficients.size(); i++) {
                addedRoot.push_back((lastValue - this->coefficients[i] * root));
                lastValue = this->coefficients[i];
            }
            addedRoot.push_back(lastValue);
            this->coefficients = addedRoot;

            this->cacheValid = false;
        }

        // (e) A method to add several roots at once.
        // 5. Member functions accepting containers should support any type of container, including array types
        template<typename Container = std::initializer_list<T>>
        void AddRoots(const Container& c)
        {
            for(auto it = std::begin(c); it != std::end(c); it++) {
                this->AddRoot(*it);
            }
        }

        // (f) A method to valuate the polynomial at a given point.
        T EvaluatePolynomial(const T x) const
        {
            /// Use lambda expressions (Chapter 6, e.g. when computing sums during evaluation of a polynomial; dispatch asynchronous computation).
            T result{};

            auto i = 0;

            // Use non-member begin and end for making it more generic.
            // Capture everything by memory as we want to write it..
            std::for_each(std::begin(this->coefficients),std::end(this->coefficients),[&](T n){ result += (n * pow(x, i)); i++; });

            /// Alternatively I could have used an ordinary for loop:
            /*for (auto i = 0; i < this->coefficients.size(); i++)
                result += (this->coefficients[i] * pow(x, i));*/
            return result;
        }

        // (g) A method to compute a polynomial which is a derivative of the polynomial.
        // (1 * c1 * x^0) + (2 * c2 * x^1) + (3 * c3 * x^2) + ... + (n * cn * x^(n-1))
        Polynomial<T> ComputeDerivative() const
        {
            Polynomial<T> ret {};
            // Shortcut if polynomial is constant
            ret.coefficients.clear();
            double p = 1;
            int counter = 0;
            double i = 0;
            for(const auto& c : this->coefficients) {
                if(counter++ == 0) {
                    continue;
                }
                ret.coefficients.push_back(p * c);
                i++;
                p++;
            }
            return ret;
        }

        // (h) A method to compute an integral for given interval bounds.
        T ComputeIntegral(const T a, const T b) {
            // 8. Use type traits
            // Alternatively I could have used enable_if here (std::enable_if_t<!std::is_integral<T>::value, T>).
            static_assert(!std::is_integral<T>::value,"ComputeIntegral is not supported for integer types.");

            //std::lock_guard<std::mutex> g(m);
            if(cacheValid == false) {
                integralCache();
                cacheValid = true;
            }
            return this->integral->EvaluatePolynomial(b) - this->integral->EvaluatePolynomial(a);
        }

        // 6. Cache the integral data to avoid repetitive integration (Items 16, 40).
        void integralCache() {
            // Make it thread-safe.
            //std::lock_guard<std::mutex> g(m);
            std::vector<T> results;
            results.push_back(0);
            for(auto i = 0.0; i < this->coefficients.size(); i++) {
                results.push_back(this->coefficients[i] / (i+1));
            }
            this->integral = std::unique_ptr<Polynomial>{ new Polynomial{results} };
        }

        // (i) A plus operator to return a polynomial equal to a sum of two polynomials.
        Polynomial<T> operator+(const Polynomial<T>& rhs) const {
            std::vector<T> tmp;
            std::vector<T> result;

            if(this->coefficients.size() >= rhs.coefficients.size()) {
                tmp = rhs.coefficients;
                tmp.insert(tmp.end(),this->coefficients.size() - rhs.coefficients.size(), T{});
                // Could also use std::plus<T> here.
                transform(std::begin(this->coefficients),std::end(this->coefficients),std::begin(tmp),std::back_inserter(result),[](T l, T r) { return l + r; });
            }
            else {
                tmp = this->coefficients;
                tmp.insert(tmp.end(), rhs.coefficients.size() - this->coefficients.size(), T{});
                transform(std::begin(rhs.coefficients),std::end(rhs.coefficients),std::begin(tmp),std::back_inserter(result),[](T l, T r) { return l + r; });
            }

            Polynomial<T> res {result};

            return res;
        }

        // (j) A star operator to return a polynomial equal to a product of two polynomials.
        Polynomial<T> operator*(const Polynomial<T>& rhs) const {
            // Initialize vector.
            const auto newDegree = this->coefficients.size() + rhs.coefficients.size()-1;
            std::vector<T> tmp(newDegree);

            int iCount = 0;
            for(const auto& i : this->coefficients) {
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

        // Also adding a operator<< to make it easier to debug and use the library.
        friend std::ostream& operator<<(std::ostream& out, const Polynomial<T>& pol) {
            int index = 0;
            int lastIndex = pol.coefficients.size() - 1;
            for(const auto& i : pol.coefficients)
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
        //mutable std::mutex m;
        mutable bool cacheValid = false;
        std::unique_ptr<Polynomial<T> > integral {};
};



#endif // POLYNOMIAL_H
