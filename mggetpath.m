function mexgen_exe=mggetpath
% Get a path to mexgen.exe
%
% This will allow us to find mexgen.exe

% Copyright (C) 2003-2006
% Jeffrey S. Perry
% Center for Perceptual Systems
% University of Texas at Austin
%
% jsp Mon Feb 16 15:39:39 CST 2004

mggetpath_m=which('mggetpath');

if isempty(mggetpath_m)
    error('Could not find mggetpath.m');
end

% Break into parts
[p,n,x,v]=fileparts(mggetpath_m);

% Find executable
if (isunix)
    mexgen_exe=fullfile(p,['mexgen']);
elseif (ispc)
    mexgen_exe=fullfile(p,['mexgen' '.exe']);
else
    error('This platform is not supported');
end

% Make sure it exists
fid=fopen(mexgen_exe);

if fid==-1
    error('Could not find the mexgen executable.  It should be in the mexgen directory in the matlab path.');
else
    fclose (fid);
end
