#ifndef COMPLEX_H // includes guard
#define COMPLEX_H

namespace Computation
{
    class Complex
    {
        /** 
        This class stores a complex number inialised with a real and imaginary part
        */

        const int constant = 2;
        long double real,img;
        public:
        Complex(long double r, long double i); //creates a Complex number with real and img parts

        float getreal(); // Returns real part

        float getimg(); // Returns imaginary part

        Complex add(Complex c); // Adds 2 complex numbers and returns the complex sum

        Complex multiply(Complex c); // Multiplies 2 complex numbers and returns the complex product

        Complex square(); // Squares a complex number.

        float magnitude(); // Returns the magnitude of the complex number
    };
}

#endif /* COMPLEX_H */