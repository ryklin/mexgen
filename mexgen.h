// Mexgen mexfile generator
//
// Copyright (C) 2003-2006
// Jeffrey S. Perry
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Wed Aug  2 14:11:32 CDT 2006

#ifndef MEXGEN_H
#define MEXGEN_H

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#define MEXGEN_MAJOR_VERSION 1
#define MEXGEN_MINOR_VERSION 0

namespace MatlabTools
{

class MexGenerator
{
    public:
        enum MatlabType
        {
            DOUBLE, COMPLEX,
            LOGICAL, CHAR, SINGLE,
            INT8, UINT8, INT16, UINT16,
            INT32, UINT32, INT64, UINT64,
            ANY, VARARGIN, VARARGOUT
        };
        struct Function
        {
            std::string name;
            std::string mfilename;
            std::string comment;
            std::vector<MatlabType> lhs;
            std::vector<MatlabType> rhs;
            std::string handler;
            bool rhs_same_sizes;
        };
        // Do not include the extension in the entry_point.
        // e.g.: MexGenerator mg ("mymexentry");
        // will create "mymexentry.cpp" and "mymexentry.dll"
        MexGenerator (const std::string &entry_point) { this->entry_point = entry_point; }
        MexGenerator () { this->entry_point = ""; }
        void SetEntryPoint (const std::string &entry_point) { this->entry_point = entry_point; }
        // There should only be one handler file, e.g.: "myhandler.cpp";
        void SetHandler (const std::string &handler) { this->handler = handler; }
        // Separate includes by spaces be sure to include \"'s or <>'s.
        // e.g.: SetIncludes ("\"myinclude1.h\" <windows.h>");
        void SetIncludes (const std::string &includes) { this->includes = includes; }
        // Separate source filenames by spaces.
        // e.g.: SetSources ("myfile1.c myfile2.cpp");
        void SetSources (const std::string &sources) { this->sources = sources; }
        // Separate library filenames by spaces.
        // e.g.: SetLibs ("c:\\mssdk\\lib\\dxguid.lib c:\\mssdk\\lib\\dinput.lib");
        void SetLibs (const std::string &libs) { this->libs = libs; }
        // Be sure to include mex.bat parameters in the command line.
        // e.g.: SetMexCommand ("c:\\matlab_sv12\\bin\\win32\\mex.bat -Ic:\\include");
        void SetMexCommand (const std::string &mex_command) { this->mex_command = mex_command; }
        // You can add includes without have to know where mex.bat lives
        // e.g.: AddPath ("c:\\include");
        void AddPath (const std::string &path) { this->include_paths.push_back (path); }
        // The vectors 'rhs' and 'lhs' specify the arguments to and
        // from the Matlab function.
        // e.g.:
        // c = myfunc (a, b)
        // vector<MatlabType> lhs, rhs;
        // lhs.push_back (LOGICAL);
        // rhs.push_back (DOUBLE);
        // rhs.push_back (DOUBLE);
        // AddFunction ("myfunc",
        //      "MYFUNC\tCompare size of two vectors.\n\nReturn 1 if two vectors are the same size, and return 0 otherwise.",
        //      lhs, rhs, cpp_function);
        void AddFunction (const std::string &name,
                const std::string &comment,
                std::vector<MatlabType> &lhs,
                std::vector<MatlabType> &rhs,
                const std::string &handler)
        {
            Function f;
            f.name = name;
            f.comment = comment;
            f.lhs = lhs;
            f.rhs = rhs;
            f.rhs_same_sizes = false;
            f.handler = handler;
            functions.push_back (f);
        }
        void AddFunction (const std::string &name,
                const std::string &comment,
                std::vector<MatlabType> &lhs,
                std::vector<MatlabType> &rhs,
                const std::string &handler,
                const std::string &mfilename)
        {
            Function f;
            f.name = name;
            f.comment = comment;
            f.lhs = lhs;
            f.rhs = rhs;
            f.rhs_same_sizes = false;
            f.handler = handler;
            f.mfilename = mfilename;
            functions.push_back (f);
        }
        // Set the last function to check that input are all the same dimension and size
        void SetRHSSameSizes ()
        {
            functions.back ().rhs_same_sizes = true;
        }
        // Generate M files for each function specified.
        void CreateFiles (std::ostream &os)
        {
            CreateHandlerFile (os);
            CreateEntryPointFile (os);
            CreateMFiles (os);
        }
        // Create the handler file
        void CreateHandlerFile (std::ostream &os)
        {
            // The user may not have specified a handler file
            if (handler.size () == 0)
                return;
            std::string fn = handler;
            os << "Checking for " << fn << "..." << std::endl;
            std::ifstream ifs (fn.c_str ());
            if (ifs)
            {
                os << "The file already exists and will not be overwritten" << std::endl;
                return;
            }
            os << "Creating " << fn << "..." << std::endl;
            std::ofstream ofs (fn.c_str ());
            if (!ofs)
                throw "Could not open file for writing";
            ofs << "// Mexgen generated this file on ";
            time_t t = time (0);
            ofs << ctime(&t) << std::endl;
            ofs << "// See the comments at the end of this file for some" << std::endl;
            ofs << "// examples of common MEX-API function calls." << std::endl;
            ofs << std::endl;
            // Add the includes
            ofs << "#include \"mex.h\"" << std::endl;
            // Add the function definitions
            for (unsigned n = 0; n < functions.size (); ++n)
            {
                // Add a space
                ofs << std::endl;
                WriteHandlerDefinition (ofs, n);
                ofs << "{" << std::endl;
                ofs << "    // Add your code here" << std::endl;
                ofs << std::endl;
                ofs << "    // Checks for input parameter number and type have already been made" << std::endl;
                ofs << "    // prior to this function call." << std::endl;
                ofs << std::endl;
                ofs << "    // Checks for output parameter number and type will be made after" << std::endl;
                ofs << "    // this function call." << std::endl;
                ofs << "}" << std::endl;
            }
            // Give some examples
            ofs << std::endl;
            ofs << "// Following are some examples of common MEX-API function calls." << std::endl;
            ofs << "//" << std::endl;
            ofs << "// See http://www.mathworks.com for the full documentation." << std::endl;
            ofs << std::endl;
            ofs << "// // Print a useful message to the Matlab console" << std::endl;
            ofs << "// mexPrintf (\"%s: %d\", \"Useful message\", 42);" << std::endl;
            ofs << std::endl;
            ofs << "// // Print a message and terminate mex-file execution" << std::endl;
            ofs << "// mexErrMsgTxt (\"mxErrMsgTxt will not return to caller\");" << std::endl;
            ofs << std::endl;
            ofs << "// // Determine how many elements are in the first input array" << std::endl;
            ofs << "// int size = mxGetNumberOfElements (prhs[0]);" << std::endl;
            ofs << std::endl;
            ofs << "// // Get a pointer to the first input array" << std::endl;
            ofs << "// double *in1 = mxGetPr (prhs[0]);" << std::endl;
            ofs << "// // After checking that there are at least 3 values in the array," << std::endl;
            ofs << "// // get their values" << std::endl;
            ofs << "// double v1 = in1[0];" << std::endl;
            ofs << "// double v2 = in1[1];" << std::endl;
            ofs << "// double v3 = in1[2];" << std::endl;
            ofs << std::endl;
            ofs << "// // A more concise way of getting the values from the input" << std::endl;
            ofs << "// double value = *mxGetPr (prhs[0]);" << std::endl;
            ofs << std::endl;
            ofs << "// // Determine how many dimensions are in input 2" << std::endl;
            ofs << "// int ndims = mxGetNumberOfDimensions (prhs[1]);" << std::endl;
            ofs << "// // Get input 2's dimensions" << std::endl;
            ofs << "// const int *dims = mxGetDimensions (prhs[1]);" << std::endl;
            ofs << "// // Allocate an array of complex numbers in output 1" << std::endl;
            ofs << "// plhs[0] = mxCreateNumericArray (ndims, dims, mxDOUBLE_CLASS, mxCOMPLEX);" << std::endl;
            ofs << "// // Get a pointer to output 1's array" << std::endl;
            ofs << "// double *destr = mxGetPr (plhs[0]);" << std::endl;
            ofs << "// // Put a value into it" << std::endl;
            ofs << "// *destr = sqrt (2);" << std::endl;
            ofs << std::endl;
            ofs << "// // Get a pointer to output 2's real and imaginary arrays" << std::endl;
            ofs << "// double *destr = mxGetPr (plhs[1]);" << std::endl;
            ofs << "// double *desti = mxGetPi (plhs[1]);" << std::endl;
            ofs << "// // Put values in output 2" << std::endl;
            ofs << "// complex<double> c (polar (rho, theta));" << std::endl;
            ofs << "// destr[0] = c.real (); desti[0] = c.imag ();" << std::endl;
            ofs << std::endl;
            ofs << "// // Return some text" << std::endl;
            ofs << "// const char *s = \"A useful message\";" << std::endl;
            ofs << "// plhs[0] = mxCreateCharMatrixFromStrings (1, &s);" << std::endl;
            ofs << std::endl;
            ofs << "// // Create a scalar and put a 32 bit id into it" << std::endl;
            ofs << "// const int dims[2] = { 1, 1 };" << std::endl;
            ofs << "// plhs[0] = mxCreateNumericArray (2, dims, mxUINT32_CLASS, mxREAL);" << std::endl;
            ofs << "// *mxGetData (plhs[0]) = (unsigned long) 100;" << std::endl;
            ofs << std::endl;
            ofs << "// // Concise way to allocate output 1 that's identical in size and" << std::endl;
            ofs << "// // type as input 1" << std::endl;
            ofs << "// plhs[0] = mxDuplicateArray (prhs[0]);" << std::endl;
        }
        // Create the entry point that gets called for each function
        void CreateEntryPointFile (std::ostream &os)
        {
            if (entry_point.size () == 0)
                throw "The entry point has not been set";
            std::string fn = entry_point + ".cpp";
            os << "Creating " << fn << "..." << std::endl;
            std::ofstream ofs (fn.c_str ());
            if (!ofs)
                throw "Could not open file for writing";
            // Standard disclaimer
            ofs << "// Mexgen generated this file on ";
            time_t t = time (0);
            ofs << ctime(&t);
            ofs << "// DO NOT EDIT!" << std::endl << std::endl;
            // Add the includes
            ofs << "#include \"mex.h\"" << std::endl;
            std::istringstream iss (includes);
            std::string include;
            while (iss >> include)
                ofs << "#include " << include << std::endl;
            // Add a space
            ofs << std::endl;
            for (unsigned n = 0; n < functions.size (); ++n)
            {
                WriteHandlerDeclaration (ofs, n);
                // Add a space
                ofs << std::endl;
                WriteCDefinition (ofs, n);
                ofs << "{" << std::endl;
                WriteParameterChecks (ofs, n);
                WriteHandlerCall (ofs, n);
                WriteOutputChecks (ofs, n);
                ofs << "}" << std::endl;
                // Add a space
                ofs << std::endl;
            }
            WriteEntryPoint (ofs);
        }
        // Create all the matlab functions
        void CreateMFiles (std::ostream &os)
        {
            if (entry_point.size () == 0)
                throw "The entry point has not been set";
            for (unsigned n = 0; n < functions.size (); ++n)
            {

                std::string fn;
                if (!functions[n].mfilename.empty ())
                    fn = functions[n].mfilename + ".m";
                else
                    fn = functions[n].name + ".m";
                os << "Creating " << fn << "..." << std::endl;
                std::ofstream ofs (fn.c_str ());
                if (!ofs)
                    throw "Could not open file for writing";
                // Write the function declaration
                ofs << "function ";
                if (!functions[n].lhs.empty ())
                {
                    ofs << "[";
                    for (unsigned i = 0; i < functions[n].lhs.size (); ++i)
                    {
                        if (functions[n].lhs[i] == VARARGOUT)
                        {
                            if (i + 1 != functions[n].lhs.size ())
                                throw "Only the last argument can be VARARGOUT";
                            else
                                ofs << "varargout";
                        }
                        else
                        {
                            ofs << "lhs" << i + 1;
                            if (i + 1 < functions[n].lhs.size ())
                                ofs << ", ";
                        }
                    }
                    ofs << "] = ";
                }
                ofs << functions[n].name << " (";
                for (unsigned i = 0; i < functions[n].rhs.size (); ++i)
                {
                    if (functions[n].rhs[i] == VARARGIN)
                    {
                        if (i + 1 != functions[n].rhs.size ())
                            throw "Only the last argument can be VARARGIN";
                        else
                            ofs << "varargin";
                    }
                    else
                    {
                        ofs << "rhs" << i + 1;
                    }
                    if (i + 1 < functions[n].rhs.size ())
                        ofs << ", ";
                }
                ofs << ")" << std::endl;
                // Write the comments
                std::istringstream iss (functions[n].comment);
                std::string line;
                while (std::getline (iss, line))
                {
                    // Make it a comment if it is not already one...
                    if (line[0] != '%')
                        ofs << "% ";
                    ofs << line << std::endl;
                }
                // Write a warning
                ofs << std::endl;
                ofs << "% Mexgen generated this file on ";
                time_t t = time (0);
                ofs << ctime(&t);
                ofs << "% DO NOT EDIT!" << std::endl << std::endl;
                // Write the function body
                if (!functions[n].lhs.empty ())
                {
                    ofs << "[";
                    for (unsigned i = 0; i < functions[n].lhs.size (); ++i)
                    {
                        if (functions[n].lhs[i] == VARARGOUT)
                        {
                            if (i + 1 != functions[n].lhs.size ())
                                throw "Only the last argument can be VARARGOUT";
                            else
                                ofs << "varargout{1:nargout-";
                                ofs << functions[n].lhs.size()-1;
                                ofs << "}";
                        }
                        else
                            ofs << "lhs" << i + 1;
                        if (i + 1 < functions[n].lhs.size ())
                            ofs << ", ";
                    }
                    ofs << "] = ";
                }
                // First param is the function index
                ofs << entry_point << " (" << n;
                for (unsigned j = 0; j < functions[n].rhs.size (); ++j)
                {
                    if (functions[n].rhs[j] == VARARGIN)
                        ofs << ", varargin{:}";
                    else
                        ofs << ", rhs" << j + 1;
                }
                // Follow by a ; to suppress output
                ofs << ");" << std::endl;
            }
        }
        void BuildTarget (std::ostream &os)
        {
            if (entry_point.size () == 0)
                throw "The entry point has not been set";
            if (mex_command.size () == 0)
                throw "The MEX command has not been set";
            std::string command;
            command += mex_command;
            if (include_paths.size () > 0)
            {
                for (unsigned i = 0; i < include_paths.size (); ++i)
                    command += " -I" + include_paths[i];
            }
            command += " " + entry_point + ".cpp";
            if (handler.size () > 0)
                command += " " + handler;
            if (sources.size () > 0)
                command += " " + sources;
            if (libs.size () > 0)
                command += " " + libs;
            os << "Executing " << command << std::endl;
            int ret = system (command.c_str ());
            if (ret)
                throw "Compilation failed";
        }
    private:
//        // Try different mex commands
//        bool TryCommand (std::string cmd)
//        {
//            //std::cout << cmd.c_str () << std::endl;
//            std::ifstream ifs (cmd.c_str ());
//            if (ifs)
//            {
//                // Wrap quotes around it in case it contains spaces
//                mex_command = "\"";
//                mex_command += cmd;
//                mex_command += "\"";
//                return true;
//            }
//            else
//                return false;
//        }
//        // Get defaults for this system
//        void SetDefaults ()
//        {
//            std::vector<std::string> cmds;
//#ifdef _WIN32
//            cmds.push_back ("c:/Program Files/MATLAB71/bin/win32/mex.bat");
//            cmds.push_back ("c:/Program Files (x86)/MATLAB71/bin/win32/mex.bat");
//            cmds.push_back ("c:/Program Files/MATLAB704/bin/win32/mex.bat");
//            cmds.push_back ("c:/Program Files (x86)/MATLAB704/bin/win32/mex.bat");
//            cmds.push_back ("c:/MATLAB6p5/bin/win32/mex.bat");
//            cmds.push_back ("c:/matlab_sv13/bin/win32/mex.bat");
//            cmds.push_back ("c:/MATLAB6p1/bin/win32/mex.bat");
//            cmds.push_back ("c:/matlab_sv12/bin/win32/mex.bat");
//            cmds.push_back ("c:/matlab_sv701/bin/win32/mex.bat");
//#endif
//#if __CYGWIN__
//            cmds.push_back ("/cygdrive/c/Program Files/MATLAB71/bin/win32/mex.bat");
//            cmds.push_back ("/cygdrive/c/Program Files (x86)/MATLAB71/bin/win32/mex.bat");
//            cmds.push_back ("/cygdrive/c/Program Files/MATLAB704/bin/win32/mex.bat");
//            cmds.push_back ("/cygdrive/c/Program Files (x86)/MATLAB704/bin/win32/mex.bat");
//            cmds.push_back ("/cygdrive/c/MATLAB6p5/bin/win32/mex.bat");
//            cmds.push_back ("/cygdrive/c/matlab_sv13/bin/win32/mex.bat");
//            cmds.push_back ("/cygdrive/c/MATLAB6p1/bin/win32/mex.bat");
//            cmds.push_back ("/cygdrive/c/matlab_sv12/bin/win32/mex.bat");
//            cmds.push_back ("/cygdrive/c/matlab_sv701/bin/win32/mex.bat");
//#endif
//            for (unsigned n = 0; n < cmds.size (); ++n)
//                if (TryCommand (cmds[n]))
//                    break;
//        }
        void WriteHandlerDeclaration (std::ostream &os, int n)
        {
            os << "void " << functions[n].handler << " (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);" << std::endl;
        }
        void WriteCDefinition (std::ostream &os, int n)
        {
            os << "void " << functions[n].name << " (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])" << std::endl;
        }
        void WriteHandlerDefinition (std::ostream &os, int n)
        {
            os << "void " << functions[n].handler << " (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])" << std::endl;
        }
        void WriteParameterChecks (std::ostream &os, int n)
        {
            if (!functions[n].rhs.empty () && functions[n].rhs.back () == VARARGIN)
            {
                os << "    // Check inputs" << std::endl;
                os << "    if (nrhs < " << functions[n].rhs.size () - 1 << ")" << std::endl;
                os << "        mexErrMsgTxt (\"This function requires at least ";
                os << functions[n].rhs.size () - 1 << " arguments.\");" << std::endl;
            }
            else
            {
                os << "    // Check inputs" << std::endl;
                os << "    if (nrhs != " << functions[n].rhs.size () << ")" << std::endl;
                os << "        mexErrMsgTxt (\"This function requires ";
                os << functions[n].rhs.size () << " arguments.\");" << std::endl;
            }
            os << std::endl;
            os << "    // Check outputs" << std::endl;
            if (!functions[n].lhs.empty () && functions[n].lhs.back () == VARARGOUT)
            {
                os << "    if (nlhs < " << functions[n].lhs.size () - 1 << ")" << std::endl;
                os << "        mexErrMsgTxt (\"This function requires at least ";
                os << functions[n].lhs.size () - 1 << " outputs.\");" << std::endl;
            }
            else
            {
                // If no LHS is specified, you can still store an answer
                // in 'ans'...
                if (functions[n].lhs.size () <= 1)
                {
                    os << "    if (nlhs > 1)" << std::endl;
                    os << "        mexErrMsgTxt (\"Too many return values were specified.\");" << std::endl;
                    os << std::endl;
                }
                // ... otherwise, make sure the number of lhs's match.
                else
                {
                    os << "    if (nlhs != " << functions[n].lhs.size () << ")" << std::endl;
                    os << "        mexErrMsgTxt (\"This function requires ";
                    os << functions[n].lhs.size () << " return values.\");" << std::endl;
                    os << std::endl;
                }
            }
            // Make sure inputs have the right type
            os << "    // Check input types" << std::endl;
            for (unsigned i = 0; i < functions[n].rhs.size (); ++i)
            {
                switch (functions[n].rhs[i])
                {
                    case DOUBLE:
                        os << "    if (!mxIsDouble (prhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Input " << i+1 << " must be double.\");" << std::endl;
                        os << "    if (mxIsComplex (prhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Input " << i+1 << " may not be complex.\");" << std::endl;
                    break;
                    case COMPLEX:
                        os << "    if (!mxIsComplex (prhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Input " << i+1 << " must be complex.\");" << std::endl;
                    break;
                    case LOGICAL:
                        os << "    if (!mxIsLogical (prhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Input " << i+1 << " must be logical.\");" << std::endl;
                    break;
                    case CHAR:
                        os << "    if (!mxIsChar (prhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Input " << i+1 << " must be char.\");" << std::endl;
                    break;
                    case SINGLE:
                        os << "    if (!mxIsSingle (prhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Input " << i+1 << " must be single.\");" << std::endl;
                    break;
                    case INT8:
                        os << "    if (!mxIsInt8 (prhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Input " << i+1 << " must be int8.\");" << std::endl;
                    break;
                    case UINT8:
                        os << "    if (!mxIsUint8 (prhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Input " << i+1 << " must be uint8.\");" << std::endl;
                    break;
                    case INT16:
                        os << "    if (!mxIsInt16 (prhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Input " << i+1 << " must be int16.\");" << std::endl;
                    break;
                    case UINT16:
                        os << "    if (!mxIsUint16 (prhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Input " << i+1 << " must be uint16.\");" << std::endl;
                    break;
                    case INT32:
                        os << "    if (!mxIsInt32 (prhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Input " << i+1 << " must be int32.\");" << std::endl;
                    break;
                    case UINT32:
                        os << "    if (!mxIsUint32 (prhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Input " << i+1 << " must be uint32.\");" << std::endl;
                    break;
                    case INT64:
                        os << "    if (!mxIsInt64 (prhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Input " << i+1 << " must be int64.\");" << std::endl;
                    break;
                    case UINT64:
                        os << "    if (!mxIsUint64 (prhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Input " << i+1 << " must be uint64.\");" << std::endl;
                    break;
                    case ANY:
                    case VARARGIN:
                    // No parameter check
                    break;
                    default:
                    throw "An unsupported input type was specified";
                }
                os << std::endl;
            }
            // Make sure all rhs arrays are the same size, if so specified
            if (!functions[n].rhs.empty () && functions[n].rhs_same_sizes)
            {
                os << "    // Check array sizes" << std::endl;
                os << "    int ndims = mxGetNumberOfDimensions (prhs[0]);" << std::endl;
                os << std::endl;
                os << "    for (int n = 0; n < nrhs; ++n)" << std::endl;
                os << "        if (mxGetNumberOfDimensions (prhs[n]) != ndims)" << std::endl;
                os << "            mexErrMsgTxt (\"The rhs's must all be the same dimension.\");" << std::endl;
                os << std::endl;
                os << "    const int *dims = mxGetDimensions (prhs[0]);" << std::endl;
                os << std::endl;
                os << "    for (n = 0; n < nrhs; ++n)" << std::endl;
                os << "    {" << std::endl;
                os << "        const int *cdims = mxGetDimensions (prhs[n]);" << std::endl;
                os << "        for (int i = 0; i < ndims; ++i)" << std::endl;
                os << "            if (dims[i] != cdims[i])" << std::endl;
                os << "                mexErrMsgTxt (\"The rhs's must all be the same size.\");" << std::endl;
                os << "    }" << std::endl;
                os << std::endl;
            }
        }
        void WriteOutputChecks (std::ostream &os, int n)
        {
            // Make sure outputs have the right type
            os << "    // Check output types" << std::endl;
            for (unsigned i = 0; i < functions[n].lhs.size (); ++i)
            {
                os << "    if (!plhs[" << i << "])" << std::endl;
                os << "        mexErrMsgTxt (\"Output " << i+1 << " was not allocated.\");" << std::endl;
                switch (functions[n].lhs[i])
                {
                    case DOUBLE:
                        os << "    if (!mxIsDouble (plhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Output " << i+1 << " must be double.\");" << std::endl;
                        os << "    if (mxIsComplex (plhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Output " << i+1 << " may not be complex.\");" << std::endl;
                    break;
                    case COMPLEX:
                        os << "    if (!mxIsComplex (plhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Output " << i+1 << " must be complex.\");" << std::endl;
                    break;
                    case LOGICAL:
                        os << "    if (!mxIsLogical (plhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Output " << i+1 << " must be logical.\");" << std::endl;
                    break;
                    case CHAR:
                        os << "    if (!mxIsChar (plhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Output " << i+1 << " must be char.\");" << std::endl;
                    break;
                    case SINGLE:
                        os << "    if (!mxIsSingle (plhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Output " << i+1 << " must be single.\");" << std::endl;
                    break;
                    case INT8:
                        os << "    if (!mxIsInt8 (plhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Output " << i+1 << " must be int8.\");" << std::endl;
                    break;
                    case UINT8:
                        os << "    if (!mxIsUint8 (plhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Output " << i+1 << " must be uint8.\");" << std::endl;
                    break;
                    case INT16:
                        os << "    if (!mxIsInt16 (plhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Output " << i+1 << " must be int16.\");" << std::endl;
                    break;
                    case UINT16:
                        os << "    if (!mxIsUint16 (plhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Output " << i+1 << " must be uint16.\");" << std::endl;
                    break;
                    case INT32:
                        os << "    if (!mxIsInt32 (plhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Output " << i+1 << " must be int32.\");" << std::endl;
                    break;
                    case UINT32:
                        os << "    if (!mxIsUint32 (plhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Output " << i+1 << " must be uint32.\");" << std::endl;
                    break;
                    case INT64:
                        os << "    if (!mxIsInt64 (plhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Output " << i+1 << " must be int64.\");" << std::endl;
                    break;
                    case UINT64:
                        os << "    if (!mxIsUint64 (plhs[" << i << "]))" << std::endl;
                        os << "        mexErrMsgTxt (\"Output " << i+1 << " must be uint64.\");" << std::endl;
                    break;
                    case ANY:
                    case VARARGOUT:
                    // No parameter check
                    break;
                    default:
                    throw "An unsupported output type was specified";
                }
                os << std::endl;
            }
        }
        void WriteHandlerCall (std::ostream &os, int n)
        {
            os << "    " << functions[n].handler << " (nlhs, plhs, nrhs, prhs);" << std::endl;
            os << std::endl;
        }
        void WriteEntryPoint (std::ostream &os)
        {
            os << "typedef void (*MEXFUNCTION) (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);" << std::endl;
            os << std::endl;
            os << "MEXFUNCTION functions[] =" << std::endl;
            os << "{" << std::endl;
            for (unsigned n = 0; n < functions.size (); ++n)
                os << "    " << functions[n].name << "," << std::endl;
            if (functions.empty ())
                os << "0";
            os << "};" << std::endl;
            os << std::endl;
            os << "static const int MAX_FUNCTIONS = sizeof (functions) / sizeof (void (*) ());" << std::endl;
            os << std::endl;
            os << "void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])" << std::endl;
            os << "{" << std::endl;
#ifndef NDEBUG
            //os << "    mexPrintf (\"%d functions defined.\\n\", MAX_FUNCTIONS);" << std::endl;
#endif
            os << "    if (nrhs < 1)" << std::endl;
            os << "        mexErrMsgTxt (\"Incorrect number of arguments.\\n\");" << std::endl;
            os << std::endl;
            os << "    int mrows = mxGetM (prhs[0]);" << std::endl;
            os << "    int ncols = mxGetN (prhs[0]);" << std::endl;
            os << std::endl;
            os << "    if (!mxIsDouble (prhs[0]) || mxIsComplex (prhs[0]) || !(mrows == 1 && ncols == 1))" << std::endl;
            os << "        mexErrMsgTxt (\"Input must be a noncomplex scalar double.\\n\");" << std::endl;
            os << std::endl;
            os << "    int findex = (int) (*mxGetPr (prhs[0]));" << std::endl;
            os << std::endl;
            os << "    if (findex < 0 || findex >= MAX_FUNCTIONS)" << std::endl;
            os << "        mexErrMsgTxt (\"Invalid function index.\\n\");" << std::endl;
            os << std::endl;
            // Remove the index of the function by subtracting it off of the lhs
            os << "    functions[findex] (nlhs, plhs, nrhs - 1, prhs + 1);" << std::endl;
            os << "}" << std::endl;
        }
        std::string handler;
        std::string entry_point;
        std::string header;
        std::string includes;
        std::string sources;
        std::string libs;
        std::string mex_command;
        std::vector<std::string> include_paths;
        std::vector<Function> functions;
};

}; // namespace MatlabTools

#endif // MEXGEN_H
