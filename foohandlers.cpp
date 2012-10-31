// foo library handlers for Mexgen mexfile generator
//
// Copyright (C) 2003-2006
// Jeffrey S. Perry
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Wed Aug  2 14:10:27 CDT 2006

#include "mex.h"
#include "foo.h"

void fooinit_cpp (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (!FOO_Init ())
        mexErrMsgTxt ("Could not initialize foo library.");
}

void foosumarray_cpp (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    plhs[0] = mxDuplicateArray (prhs[0]);

    double *src0 = mxGetPr (prhs[0]);
    double *src1 = mxGetPr (prhs[1]);
    double *dest = mxGetPr (plhs[0]);

    int size = mxGetNumberOfElements (prhs[0]);

    if (!FOO_SumArray (size, dest, src0, src1))
        mexErrMsgTxt ("FOO_SumArray failed.");
}

void foopolartocomplex_cpp (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    double *srcmag = mxGetPr (prhs[0]);
    double *srcangle = mxGetPr (prhs[1]);

    int ndims = mxGetNumberOfDimensions (prhs[0]);
    const int *dims = mxGetDimensions (prhs[0]);

    plhs[0] = mxCreateNumericArray (ndims, dims, mxDOUBLE_CLASS, mxCOMPLEX);

    double *destr = mxGetPr (plhs[0]);
    double *desti = mxGetPi (plhs[0]);

    int size = mxGetNumberOfElements (prhs[0]);

    if (!FOO_PolarToComplex (size, destr, desti, srcmag, srcangle))
        mexErrMsgTxt ("FOO_SumArray failed.");
}
