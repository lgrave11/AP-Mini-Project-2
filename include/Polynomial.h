#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#include <iostream>
#include <vector>
#include <iostream>
#include <initializer_list>

namespace PolynomialLib
{
    // Forward declaration is required it is used in the operator forward declarations.
    template<typename T> class Polynomial;
    // Forward declarations for operator overloads. This is a choice, alternatively I could also have used a different template typename.
    template<typename T> std::ostream& operator<< (std::ostream&, const Polynomial<T>&);

    // T = valuetype, e.g double, int or complex
    // C = container type, e.g array, vector or whatever. Just the iterator.
    template<typename T> class Polynomial
    {
        public:
            Polynomial();
            //template<typename C>
            template<typename Container>
            Polynomial(Container&);
            Polynomial(const Polynomial<T>& oPoly); // Copy constructor
            Polynomial(Polynomial<T>&& oPoly); // Move constructor
            Polynomial& operator= (const Polynomial<T>& oPoly); // Copy assignment operator.
            Polynomial& operator= (Polynomial<T>&& oPoly); // Move assignment operator.
            virtual ~Polynomial();

            void Scale(const T); // OK
            void AddRoot(const T);
            //template<typename C>
            template<typename Container>
            void AddRoots(const Container&);
            T EvaluatePolynomial(const T); // OK
            T ComputeDerivative(const T); // OK
            T ComputeIntegral(const T, const T); // OK

            // Operators
            Polynomial<T> operator+(const Polynomial<T>&); // OK
            Polynomial<T> operator*(const Polynomial<T>&);
            friend std::ostream& operator<< <>(std::ostream&, const Polynomial<T>&); // <> required to tell operator<< that this is a template function.

            int degree;
            std::vector<T> coefficients;
        protected:
        private:
    };
}



#endif // POLYNOMIAL_H
