// Mexgen mexfile generator for the foo library
//
// Copyright (C) 2003-2006
// Jeffrey S. Perry
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Wed Aug  2 14:10:45 CDT 2006

#include "mexgen.h"
#include "foo.h"

using namespace std;
using namespace MatlabTools;

int main ()
{
    try
    {
        // The generator gets init'ed with the name of the Matlab mex
        // file to generate.
        MexGenerator mg ("foomex");

        // All of our source
        mg.SetSources ("foohandlers.cpp foo.cpp");

#ifndef NDEBUG
        mg.SetMexCommand ("\"c:\\Program Files (x86)\\MATLAB71\\bin\\win32\\mex.bat\" -g");
#else
        mg.SetMexCommand ("\"c:\\Program Files (x86)\\MATLAB71\\bin\\win32\\mex.bat\"");
#endif

        vector<MexGenerator::MatlabType> lhs, rhs;

        mg.AddFunction ("fooinit",
            "FOOINIT\tInitialize the foo library.\n"
            "\n"
            "Call this before calling other foo routines.",
            lhs, rhs, "fooinit_cpp");
        rhs.push_back (MexGenerator::DOUBLE); // src1 = array of doubles
        rhs.push_back (MexGenerator::DOUBLE); // src2 = array of doubles
        lhs.push_back (MexGenerator::DOUBLE); // dest = array of doubles

        mg.AddFunction ("foosumarray",
            "FOOSUMARRAY\tSum 2 arrays.",
            lhs, rhs, "foosumarray_cpp");
        mg.SetRHSSameSizes ();
        rhs.clear ();
        lhs.clear ();
        rhs.push_back (MexGenerator::DOUBLE); // srcmags = array of doubles
        rhs.push_back (MexGenerator::DOUBLE); // srcangles = array of doubles
        lhs.clear ();
        lhs.push_back (MexGenerator::COMPLEX); // dest = array of complexes

        mg.AddFunction ("foopolartocomplex",
            "FOOPOLARTOCOMPLEX\tConvert from polar to complex.\n"
            "\n"
            "Convert an array of magnitudes and an array\n"
            "of angles to an array of complex numbers.",
            lhs, rhs, "foopolartocomplex_cpp");
        mg.SetRHSSameSizes ();

        mg.CreateFiles (cout);
        mg.BuildTarget (cout);

        return 0;
    }
    catch (const char *e)
    {
        cerr << "Exception: " << e << endl;
        return -1;
    }
}
