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

// T = valuetype, e.g double, int or complex
// C = container type, e.g array, vector or whatever. Just the iterator.
template<typename T>
class Polynomial
{
    public:
        std::vector<T> coefficients;

        Polynomial() : coefficients { std::vector<T> {0} } { }
        virtual ~Polynomial() {}
        Polynomial(const Polynomial<T>& oPoly) = delete; // Copy constructor
        Polynomial& operator= (const Polynomial<T>& oPoly) = delete; // Copy assignment operator.
        Polynomial(Polynomial<T>&& oPoly) : coefficients(std::move(oPoly.coefficients)) {}// Move constructor
        Polynomial& operator= (Polynomial<T>&& oPoly); // Move assignment operator.

        // Following requirement 5 here and with AddRoots to accept any container. Including array types.
        template<typename Container>
        Polynomial(const Container& c) : coefficients { std::vector<T> {} } {
            // Following requirement 3, to use auto where applicable.
            for(auto it = std::cbegin(c); it != std::cend(c); it++) {
                this->coefficients.emplace_back(*it);
            }
        }

        // Support brace initialization using constructor forwarding.
        //Polynomial(const std::initializer_list<T> c) : Polynomial{ std::vector<T> {c} } {}
        // Support brace initialization with a regular constructor.
        Polynomial(const std::initializer_list<T> c) : coefficients{ std::vector<T> {c} } {}

        // Following requirement 4 to use const.
        void Scale(const T scalar) {
            //std::lock_guard<std::mutex> g(m);
            std::vector<T> scaledCoefficients;
            // Use const reference because we are not modifying i.
            // Also use auto&. Could also just use auto i.
            /*for(const auto& i : this->coefficients) {
                auto result = i * scalar;
                scaledCoefficients.push_back(result);
            }*/
            /// Use std::transform with a lambda instead of a traditional loop. Requirement 9: Use lambda expressions.
            std::transform(std::begin(this->coefficients), std::end(this->coefficients), std::back_inserter(scaledCoefficients), [&scalar](const T& i) {return i * scalar;});
            this->coefficients = scaledCoefficients;

            this->cacheValid = false;
        }

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

        template<typename Container = std::initializer_list<T>>
        void AddRoots(const Container& c)
        {
            for(auto it = std::begin(c); it != std::end(c); it++) {
                this->AddRoot(*it);
            }
        }

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

        // (1 * c1 * x^0) + (2 * c2 * x^1) + (3 * c3 * x^2) + ... + (n * cn * x^(n-1))
        Polynomial ComputeDerivative() const
        {
            Polynomial ret {};
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


            /*T sum{};
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
            return sum;*/
        }

        /// 6. Cache the integral data to avoid repetitive integration (Items 16, 40). Make it thread-safe.
        void integralCache() {
            std::vector<T> results;
            results.push_back(0);
            for(auto i = 0.0; i < this->coefficients.size(); i++) {
                results.push_back(this->coefficients[i] / (i+1));
            }
            this->integral = std::unique_ptr<Polynomial>{ new Polynomial{results} };
        }

        T ComputeIntegral(const T a, const T b) {
            /// Requirement 8: Use type traits (see examples in Item 27, e.g. disable or fail assertion for the integration method over integer types).
            // Alternatively I could have used enable_if here (std::enable_if_t<!std::is_integral<T>::value, T>).
            static_assert(!std::is_integral<T>::value,"ComputeIntegral is not supported for integer types.");

            //std::lock_guard<std::mutex> g(m);
            if(cacheValid == false) {
                integralCache();
                cacheValid = true;
            }
            return this->integral->EvaluatePolynomial(b) - this->integral->EvaluatePolynomial(a);
        }

        // Operators
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
        std::unique_ptr<Polynomial<T> > derivative {};
};



#endif // POLYNOMIAL_H
