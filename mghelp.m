% Mexgen: MATLAB External Interface Routine Generator
%
% Quick Start
% -----------
%
% After installation, change to the directory where you wish to save
% and compile the mex-files that you are about to create.  Then type
%
%   > mgprompt
%
% from the MATLAB command line.
%
% Mexgen will prompt you for the information needed to generate the M-
% and C++ files and then allow you to edit the C++ 'handler' file that
% does the actual processing.  Mexgen then tries to compile the C++
% files into a MEX-file.
%
% Each time you make changes to the C++ handler file, you need to
% recompile and link the MEX-file with the 'mex' command:
%
%   >> mex myentry.cpp myhandler.cpp
%
% where 'myentry.cpp' and 'myhandler.cpp' are the names of the files
% supplied to mgprompt.m.  NOTE that the order of the filenames given
% to mex is important.
%
% General Concepts
% ----------------
%
% Mexgen is a command-line utility that generates various MATLAB
% external interface routine files by interpreting commands from a
% mexgen script file.
%
% Mexgen script files typically have the extension '.mg'.  Two such
% example scripts are available in the mexgen directory: abc.mg and
% xyz.mg.  abc.mg is a very simple example.  xyz.mg is a more
% complicated example.
%
% mgprompt.m is a matlab function that prompts the user for
% information about the mex-files that the user wishes to generate.
% It then creates the files, allows the user to edit the C++ 'handler'
% file that does the actually processing, and then mexgen tries to
% compile the C++ files in order to create a MATLAB callable
% executable, or MEX-file.
%
% When mexgen is run for the first time, it typically creates three
% types of files:  M-files, a C++ entry point file, and a C++ handler
% file.
%
% M-Files
% -------
%
% An M-file will be created for each function specified in the mexgen
% script.  These files contain the comments for the function specified
% in the mexgen file.  These files also each contain a single line of
% code: a call to the MEX-file, passing the function arguments and an
% index that is used to indicate which function is being called.
%
% Do not edit mexgen generated M-files as these files will get
% overwritten on subsequent calls to mexgen.
%
% C++ Entry Point File
% --------------------
%
% The C++ entry point file will eventually be compiled into a MATLAB
% callable executable.  This executable gets called from the generated
% M-files, passing the function arguments and an index indicating which
% function is being called.
%
% The C++ entry point file checks arguments and return types specified
% by the mexgen script file and generates an error in the event that a
% function is called with incorrect inputs or outputs.  If the
% arguments are correct, the C++ entry point file will call the
% function in the C++ handler specified by the mexgen script.
%
% Do not edit the mexgen generated C++ entry point file as this file
% will get overwritten on subsequent calls to mexgen.
%
% C++ Handler File
% ----------------
%
% The C++ handler file contains the code that does the actual
% processing of the data supplied by MATLAB.  This file should be
% edited so that the functions it contains processes and returns the
% data according to the user's needs.
%
% The generated C++ handler file also contains example C++ code that
% has been commented out.  The example code implements common
% MEX-file API calls such as examining input arrays, printing error
% diagnostics, and allocating output arrays.  The example code is
% provided so that the user may cut and paste the code as they see
% fit.
%
% This file only gets generated once.  Mexgen will never overwrite
% this file if it already exists.
%
% Code Organization
% -----------------
%
% The purpose of organizing the code in this way serves several
% purposes:
%
%  - Functions may share global data.  If you were to create a C++
%    file for each MEX function and compile them with the MATLAB 'mex'
%    command, each function would be implemented in a separate MATLAB
%    callable executable and would therefore have to rely on more
%    complicated methods in order to share global data.
%
%  - The mundane task of checking input and output number and type is
%    simplified.  Inputs and outputs are specified in the mexgen
%    script file in a syntax that is similar to the way they are
%    specified in MATLAB.  Therefore, the handler file can assume that
%    the inputs and outputs are correct and need not write any type
%    checking code or code to check the number of inputs and outputs.
%
%  - It allows all of the help documentation to reside in one single
%    '.mg' file so that it is easier to keep a group of functions'
%    help documentation consistent.
%
%  - Functions may be added/removed and argument types and numbers
%    may be easily changed.  By separating the C++ entry point file
%    from the C++ handler file, the code to do type checking and
%    handler code calling may be regenerated when the script is
%    changed without affecting the code that processes the data.
%
%    Note that when you add or remove functions, the M-files must also
%    be regenerated in order to synchronize function indices.
%
%    Note also that when a function is added or removed, its
%    definition must be added or removed manually from the C++ handler
%    file because the handler file never gets modified by mexgen after
%    being generated the first time.
%
% Feedback
% --------
%
% Please send feedback to:
%
%   jsp@mail.utexas.edu
%
% Copyright (C) 2003-2006
% Jeffrey S. Perry
% Center for Perceptual Systems
% University of Texas at Austin

% jsp Mon Apr  5 18:35:40 CDT 2004
help mghelp
