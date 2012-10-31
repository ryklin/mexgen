function mgentry(fn)
% MGENTRY(FN)
%
% Generate entry point file for the specified mexgen file.

% Copyright (C) 2003-2006
% Jeffrey S. Perry
% Center for Perceptual Systems
% University of Texas at Austin

% jsp Thu Aug  3 11:58:39 CDT 2006

mexgen_exe=mggetpath;

% Create the shell command
cmd=sprintf('!"%s" -e "%s"',mexgen_exe,fn);

fprintf ('Evaluating %s\n', cmd);

% Do it
eval(cmd)

disp('Done');
