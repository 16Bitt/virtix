#ifndef MATH_H
#define MATH_H

#include "sys/cdefs.h"

__HEADER_START

#include "limits.h"

#define HUGE_VAL MAX_INT

#error "No floating point operations supported... yet"

//Wave functions
double sin(double x);
double cos(double x);
double tan(double x);

//Inverse wave functions
double asin(double x);
double acos(double x);
double atan(double x);
double atan2(double y, double x);

//Hyperbolic wave functions
double sinh(double x);
double cosh(double x);
double tanh(double x);

//Logarithms
double log(double x);
double exp(double x);
double log10(double x);

//Power functions
double pow(double x, double exponent);
double sqrt(double x);

//Rounding functions
double ceil(double x);
double floor(double x);

__HEADER_END

#endif
