% Example of a minimal functionality Mexgen file
%
% Copyright (C) 2003-2006
% Jeffrey S. Perry
% Center for Perceptual Systems
% University of Texas at Austin
%
% jsp Mon Feb  9 14:26:34 CST 2004

function abcdonothing
% ABCDONOTHING
%
% This function does nothing.
end

function [uint8]=abcclamp(double)
% IMAGE=ABCCLAMP(X)
%
% Clamp the values in X to the range 0-255.  That is, set values
% greater than 255 to 255, and set values less than 0 to 0.
%
% Return the clamped values as an 8 bit grayscale image.
end

% abchandlers.cpp will contain the actual C++ code that does the
% processing.  If this file already exists, it will NOT be
% overwrittten.  This file, once generated, should be edited by the
% user to do the actual processing.
handlers('abchandlers.cpp');

% Set the entry point so that the following files may be created:
%
%   abcentry.cpp    Entry code that checks function parameter types
%                   return value types.
%   abcentry.dll    Compiled code for abcentry.cpp.
entrypoint('abcentry');
