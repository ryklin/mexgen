// Foo library to test Mexgen mexfile generator
//
// Copyright (C) 2003-2012
// Jeffrey S. Perry
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Wed Aug  2 14:10:55 CDT 2006

#include "foo.h"
#include <complex>

using namespace std;

int FOO_Init ()
{
    return 1;
}

int FOO_SumArray (int size, double *dest, double *src1, double *src2)
{
    for (int n = 0; n < size; ++n)
        dest[n] = src1[n] + src2[n];
    return 1; // Success
}

int FOO_PolarToComplex (int size, double *destr, double *desti, double *srcrho, double *srctheta)
{
    for (int n = 0; n < size; ++n)
    {
        // Convert mag, angle to a complex
        complex<double> c (polar (srcrho[n], srctheta[n]));
        destr[n] = c.real ();
        desti[n] = c.imag ();
    }
    return 1; // Success
}
