// Foo library to test Mexgen mexfile generator
//
// Copyright (C) 2003 Center for Perceptual Systems
//
// jsp Wed Aug  2 14:11:32 CDT 2006

#ifndef FOO_H
#define FOO_H

// Initialize the library.
//
// Return 1 for success
int FOO_Init ();

// Sum 'src1' and 'src2' and put the result in 'dest'.
//
// Return 1 for success
int FOO_SumArray (int n, double *dest, double *src1, double *src2);

// Convert 'srcrho' and 'srctheta' from polar to complex coordinates,
// and put the result in 'destr' and 'desti'.
//
// Return 1 for success
int FOO_PolarToComplex (int n, double *destr, double *desti, double *srcrho, double *srctheta);

#endif FOO_H
