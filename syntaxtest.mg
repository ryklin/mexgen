% Syntax test Mexgen file
%
% This file checks mexgen's lexical analyzer and parser.
%
% Copyright (C) 2003-2006
% Jeffrey S. Perry
% Center for Perceptual Systems
% University of Texas at Austin
%
% jsp Wed Feb 11 14:23:28 CST 2004

%
% comment
%

minversion(0.0)
handlers('testhandlers') handlers('testhandlers')
sources('testsource1.cpp testsource2.cpp testsource3.cpp');sources('');

sources('testsource1.cpp testsource2.cpp testsource3.cpp');sources(''); % comment
libs('testlib.lib');

mexcommand('c:\\matlab\\mex.bat');
addpath('c:\\includes');
addpath('c:\\more_includes');

entrypoint('zzz')

% valid types are
% double
% complex
% logical
% char
% single
% int8
% uint8
% int16
% uint16
% int32
% uint32
% int64
% uint64
% any
% varargin
% varargout

function test end % comment
function double=test end function [complex]=test end
function [logical,single]=test end
function test(char) end
function test(int8,uint8) end
function [int16,uint16]=test(int32,uint32,any) end
function [int8,varargout]=test(int32,uint32,any) end
function [int16,uint16]=test(int32,varargin) end

function test(int32,varargin) %
% Test comments
%
end

function test(int32,varargin)
% Test comments
%
end
function test(int32,varargin) % Test comments
end

handlers('testhandlers')
entrypoint('zzz')

% the end
