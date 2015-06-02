#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#include <iostream>
#include <vector>
#include <iostream>
#include <initializer_list>

// T = valuetype, e.g double, int or complex
// C = container type, e.g array, vector or whatever. Just the iterator.
template<typename T> class Polynomial
{
    public:
        Polynomial() {}
        virtual ~Polynomial() {}
        Polynomial(const Polynomial<T>& oPoly) : coefficients(oPoly.coefficients), degree(oPoly.degree) {} // Copy constructor
        //Polynomial(Polynomial<T>&& oPoly); // Move constructor
        //Polynomial& operator= (const Polynomial<T>& oPoly); // Copy assignment operator.
        //Polynomial& operator= (Polynomial<T>&& oPoly); // Move assignment operator.
        template<typename Iterator>
        Polynomial(Iterator begin, Iterator end) {
            for(auto it = begin; it != end; it++) {
            //for(const auto& item : coeffs) {
                this->coefficients.emplace_back(*it);
                //this->coefficients.push_back(item);
            }
            this->degree = this->coefficients.size();
        }



        void Scale(const T scalar) {
            std::vector<T> scaledCoefficients;
            // Use const reference because we are not modifying i.
            // Also use auto&. Could also just use auto i.
            for(const auto& i : this->coefficients) {
                auto result = i * scalar;
                scaledCoefficients.push_back(result);
            }
            //std::transform(std::begin(this->coefficients), std::end(this->coefficients), std::begin(scaledCoefficients), std::bind1st(std::multiplies<T>(), scalar));
            this->coefficients = scaledCoefficients;
        } // OK
        void AddRoot(const T root) {
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
        template<typename Iterator>
        void AddRoots(Iterator begin, Iterator end)
        {
            for(auto it = begin; it != end; it++) {
                this->AddRoot(*it);
            }
            //for(const auto& i : roots) {
            //    this->AddRoot(i);
            //}
        }
        T EvaluatePolynomial(const T x)
        {
            /// Use lambda expressions (Chapter 6, e.g. when computing sums during evaluation of a polynomial; dispatch asynchronous computation).
            T result{};

            auto i = 0;
            // Use non-member begin and end for making it more generic.
            // Capture everything, because we need most of everything.
            std::for_each(std::begin(this->coefficients),std::end(this->coefficients),[&](T n){ result += (n * pow(x, i)); i++; });
            // Alternative could have used an ordinary for loop:
            /*for (auto i = 0; i < this->degree; i++)
                result += (this->coefficients[i] * pow(x, i));*/
            return result;
        }
        T ComputeDerivative(const T x)
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
        T ComputeIntegral(const T a, const T b)
        {
            /// Use type traits (see examples in Item 27, e.g. disable or fail assertion for the integration method over integer types).
            // Alternatively I could have used enable_if here (std::enable_if_t<!std::is_integral<T>::value, T>).
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

        // Operators
        Polynomial<T> operator+(const Polynomial<T>& rhs) {
            std::vector<T> tmp {};
            if(degree >= rhs.degree) {
                transform(this->coefficients.begin(),this->coefficients.end(),rhs.coefficients.begin(),std::back_inserter(tmp),std::plus<T>());
            }
            else {
                transform(rhs.coefficients.begin(),rhs.coefficients.end(),this->coefficients.begin(),std::back_inserter(tmp),std::plus<T>());
            }
            Polynomial<T> res {std::begin(tmp), std::end(tmp)};

            return res;
        }

        Polynomial<T> operator*(const Polynomial<T>& rhs) {
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

            Polynomial<T> res {std::begin(tmp), std::end(tmp)};

            return res;
        }

        friend std::ostream& operator<<(std::ostream& out, const Polynomial<T>& pol) {
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
        std::vector<T> coefficients;
        int degree;

    protected:
    private:
};



#endif // POLYNOMIAL_H
