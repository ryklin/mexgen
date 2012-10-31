% Example Mexgen input file
%
% This example illustrates most of the language features of mexgen.
%
% Copyright (C) 2003-2006
% Jeffrey S. Perry
% Center for Perceptual Systems
% University of Texas at Austin
%
% jsp Mon Feb  2 10:38:58 CST 2004

% Specify the minimum version of mexgen required to parse this file.
minversion(0.2)

% Now we have to specify the functions in our mex file.  For each
% function we specify, a matlab M-File will be created with the
% appropriate function name, arguments, comments, and type checking.
%
% The M-File then calls the appropriate C++ handler function.
%
% These files should never need to be edited, so they are overwritten
% if they already exist in the current directory.

% Functions are specified like this
%
%   function [outputtypes]=functionname(inputtypes)
%   % Comments
%   end
%
% Where outputtypes is an optional list of output type keywords and
% inputtypes is an optional list of type keywords.  Type keywords may
% be one of:
%
%   double
%   complex
%   logical
%   char
%   single
%   int8
%   uint8
%   int16
%   uint16
%   int32
%   uint32
%   int64
%   uint64
%   any
%
% The input type 'varargin' and the output type 'varargout' are also valid
% type keywords.

% Function definition examples follow.

function xyzinit
% Initialize the xyz library
%
% All comments listed in a function declaration are included in the
% function definition.
end

function xyzfunction1(double)
% xyzfunction1 has one double input
end

function complex=xyzfunction2(double,double)
% z=xyzfunction2(x,y)
%
% The generated code will check to make sure this function has exactly
% two double inputs and one complex output.
end

function xyzfunction3(any)
% The generated code for this function will check to make sure it has
% exactly one input, but it will not perform type checking on that
% input.
end

function xyzfunction4(uint32,varargin)
% In this function, the code generated will check to make sure there
% is at least one input of type uint32.  Type checking of any
% remaining arguments must be performed by the handler code.
end

function [double,logical,varargout]=xyzfunction5(double)
% This function has a variable number of outputs.  The generated code
% will check that the handler code allocated at least two outputs, the
% first being of type double, and the second being of type logical.
end

% The source files may contain some new or legacy code, but it will
% typically not contain the matlab interface functions.  The interface
% functions belong in the handlers file.
sources('xyz.cpp');

% We may want to change the matlab command line to add some options.
mexcommand('c:\\matlab_sv13\\bin\\win32\\mex.bat -Ic:\\includes -g')

% You can add includes without having to know where mex.bat lives
addpath('c:\\more_includes')

% Our handlers and sources may require additional libraries.
libs('c:\\mssdk\\lib\\dxguid.lib')

% Our C++ handler functions will be generated in the following file.
%
% This is just a skeleton of empty functions whose bodies we may later
% fill in.
%
% If this file already exists in the current directory, it will never
% be overwritten.  You must instead delete it explicitly.
handlers('xyzhandlers.cpp');

% The entry point file will be called xyzentry.cpp which in turn gets
% compiled to a MEX-File called xyzentry.dll.
entrypoint('xyzentry');
