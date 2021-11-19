#include<Complex.hpp>
#include<math.h>


    /** 
    This class stores a complex number inialised with a real and imaginary part
    */

    Computation::Complex::Complex(long double r, long double i) 
    {
        real = r;
        img = i;
    }

    float Computation::Complex::getreal()
    {
        return real;
    }

    float Computation::Complex::getimg()
    {
        return img;
    }

    Computation::Complex Computation::Complex::add(Complex c)
    {
        return Complex(real+c.getreal(),img+c.getimg());
    }

    Computation::Complex Computation::Complex::multiply(Complex c)
    {
        return Complex(real*c.getreal()-img*c.getimg(), img*c.getreal()+real*c.getimg());
    }

    Computation::Complex Computation::Complex::square()
    {
        return multiply(Complex(real,img));
    }

    float Computation::Complex::magnitude()
    {
        return sqrt(real*real+img+img);
    }
