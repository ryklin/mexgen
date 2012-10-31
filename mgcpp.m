function mgcpp(fn)
% MGCPP(FN)
%
% Generate a C++ handler file for the specified mexgen file.

% Copyright (C) 2003-2006
% Jeffrey S. Perry
% Center for Perceptual Systems
% University of Texas at Austin

% jsp Mon Feb 16 15:35:40 CST 2004

mexgen_exe=mggetpath;

% Create the shell command
cmd=sprintf('!"%s" -c "%s"',mexgen_exe,fn);

fprintf ('Evaluating %s\n', cmd);

% Do it
eval(cmd)

disp('Done');
