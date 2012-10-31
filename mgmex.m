function mgmex(fn)
% MGMEX(FN)
%
% Generate mex file for the specified mexgen file.
%
% NOTE: It may be preferable to run 'mex fooentry.cpp foohandlers.cpp'
% from the Matlab command line rather than calling this function.

% Copyright (C) 2003-2006
% Jeffrey S. Perry
% Center for Perceptual Systems
% University of Texas at Austin

% jsp Mon Feb 16 15:42:12 CST 2004

mexgen_exe=mggetpath;

% Create the shell command
cmd=sprintf('!"%s" -x "%s"',mexgen_exe,fn);

fprintf ('Evaluating %s\n', cmd);

% Do it
eval(cmd)

disp('Done');
