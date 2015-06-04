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
        int degree;

        Polynomial() : coefficients({}), degree{} {}
        virtual ~Polynomial() {}
        Polynomial(const Polynomial<T>& oPoly) : coefficients(oPoly.coefficients), degree(oPoly.degree) {} // Copy constructor'
        Polynomial& operator= (const Polynomial<T>& oPoly); // Copy assignment operator.
        Polynomial(Polynomial<T>&& oPoly); // Move constructor
        Polynomial& operator= (Polynomial<T>&& oPoly); // Move assignment operator.

        // Following requirement 5 here and with AddRoots to accept any container. Including array types.
        // Also support braced initializers by giving a hint to the compiler that the Container is a std::initializer_list
        template<typename Container>
        Polynomial(const Container& c) {
            // Following requirement 3, to use auto where applicable.
            for(auto it = std::begin(c); it != std::end(c); it++) {
                this->coefficients.emplace_back(*it);
            }
            this->degree = this->coefficients.size();
        }

        // Support brace initialization using delegating constructor.
        Polynomial(const std::initializer_list<T> c) : Polynomial{ std::vector<T> {c} } {}

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
            this->integral->coefficients.clear();
            this->integral->degree = 0;
            this->cacheValid = false;
        }

        void AddRoot(const T root) {
            //std::lock_guard<std::mutex> g(m);
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

            this->integral->coefficients.clear();
            this->integral->degree = 0;
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
            /*for (auto i = 0; i < this->degree; i++)
                result += (this->coefficients[i] * pow(x, i));*/
            return result;
        }

        T ComputeDerivative(const T x) const
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

        /// 6. Cache the integral data to avoid repetitive integration (Items 16, 40). Make it thread-safe.
        void integralCache() {
            std::vector<T> results;
            results.push_back(0);
            for(auto i = 0; i < this->degree; i++) {
                results.push_back(this->coefficients[i] / (i+1));
            }
            this->integral = std::unique_ptr<Polynomial>{ new Polynomial{results} };;
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
            std::cout << *this->integral << std::endl;
            return this->integral->EvaluatePolynomial(b) - this->integral->EvaluatePolynomial(a);
        }
        /*T getIntegralValue(const T val) const
        {
            // Using string as a key temporarily because complex is not easy to map, because it doesnt have a operator> or std::hash implemented so map and unorderedmap are both out.
            std::string key = std::to_string(val);
            T result {};
            std::lock_guard<std::mutex> g(m);
            if((integralCache.find(key) != integralCache.end()))
            {
                result = integralCache[key];
            }
            else {
                for (auto i=0.0; i < this->degree; i++)
                {
                    result += (this->coefficients[i] / (i+1)) * pow(val, i+1);
                }
                integralCache[key] = result;
            }
            return result;
        }

        T ComputeIntegral(const T a, const T b) const
        {
            /// Requirement 8: Use type traits (see examples in Item 27, e.g. disable or fail assertion for the integration method over integer types).
            // Alternatively I could have used enable_if here (std::enable_if_t<!std::is_integral<T>::value, T>).
            static_assert(!std::is_integral<T>::value,"ComputeIntegral is not supported for integer types.");
            T left = getIntegralValue(b);
            T right = getIntegralValue(a);

            return left - right;
        }*/



        // Operators
        Polynomial<T> operator+(const Polynomial<T>& rhs) const {
            std::vector<T> tmp;
            std::vector<T> result;

            if(this->degree >= rhs.degree) {
                tmp = rhs.coefficients;
                tmp.insert(tmp.end(),this->degree - rhs.degree, T{});
                // Could also use std::plus<T> here.
                transform(std::begin(this->coefficients),std::end(this->coefficients),std::begin(tmp),std::back_inserter(result),[](T l, T r) { return l + r; });
            }
            else {
                tmp = this->coefficients;
                tmp.insert(tmp.end(), rhs.degree - this->degree, T{});
                transform(std::begin(rhs.coefficients),std::end(rhs.coefficients),std::begin(tmp),std::back_inserter(result),[](T l, T r) { return l + r; });
            }



            // I wish I could use transform on vectors of uneven size, but that doesnt seem to work.
            /*auto i1 = std::begin(this->coefficients);
            auto i2 = std::begin(rhs.coefficients);
            if(degree >= rhs.degree) {

                for(;i1 != std::end(this->coefficients);i1++)
                {
                    if(i2 != std::end(rhs.coefficients)) {
                        tmp.push_back(*i1 + *i2);
                        i2++;
                    }
                    else {
                        tmp.push_back(*i1 + T{});
                    }
                }
            }
            else {
                for(;i2 != std::end(rhs.coefficients);i2++)
                {
                    if(i1 != std::end(this->coefficients)) {
                        tmp.push_back(*i1 + *i2);
                        i1++;
                    }
                    else {
                        tmp.push_back(*i2 + T{});
                    }
                }
            }*/


            /*if(degree >= rhs.degree) {
                // Could also use std::plus<T> here.
                transform(std::begin(this->coefficients),std::end(this->coefficients),std::begin(rhs.coefficients),std::back_inserter(tmp),[](T l, T r) { return l + r; });
            }
            else {
                transform(std::begin(rhs.coefficients),std::end(rhs.coefficients),std::begin(this->coefficients),std::back_inserter(tmp),[](T l, T r) { return l + r; });
            }*/
            Polynomial<T> res {result};

            return res;
        }

        Polynomial<T> operator*(const Polynomial<T>& rhs) const {
            // Initialize vector.
            const auto newDegree = degree + rhs.degree-1;
            std::vector<T> tmp(newDegree);

            int iCount = 0;
            for(const auto& i : coefficients) {
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
        //Polynomial integral {};
        //mutable std::unordered_map<std::string, T> integralCache{};
};



#endif // POLYNOMIAL_H
