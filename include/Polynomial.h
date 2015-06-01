#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#include <iostream>
#include <vector>
#include <iostream>
#include <initializer_list>

namespace PolynomialLib
{
    // Forward declaration is required it is used in the operator forward declarations.
    template<typename C> class Polynomial;
    // Forward declarations for operator overloads. This is a choice, alternatively I could also have used a different template typename.
    template<typename C> std::ostream& operator<< (std::ostream&, const Polynomial<C>&);

    template<typename C> class Polynomial
    {
        public:
            Polynomial();
            Polynomial(std::vector<C> coefficients);
            virtual ~Polynomial();

            void Scale(C); // OK
            void AddRoot(C);
            void AddRoots(std::vector<C>);
            C EvaluatePolynomial(C); // OK
            C ComputeDerivative(C); // OK
            C ComputeIntegral(C, C); // OK

            // Operators
            Polynomial<C> operator+(const Polynomial<C>&); // OK
            Polynomial<C> operator*(const Polynomial<C>&);
            friend std::ostream& operator<< <>(std::ostream&, const Polynomial<C>&); // <> required to tell operator<< that this is a template function.

            int degree;
            std::vector<C> coefficients;
        protected:
        private:
    };
}



#endif // POLYNOMIAL_H
