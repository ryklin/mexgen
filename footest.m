function footest ()
% FOOTEST
%
% Test the foo library
%
% This test calls some trivial functions in a CPP libary called 'foo'.

% Copyright (C) 2003-2006
% Jeffrey S. Perry
% Center for Perceptual Systems
% University of Texas at Austin
%
% jsp Mon Feb  2 10:39:28 CST 2004

disp('Testing fooinit...');
fooinit
disp('Testing foosumarray...');
size=10;
a=2*ones(size);
b=5*ones(size);
c=foosumarray(a,b);
if (not (c == 7*ones(size)))
    error ('foosumarray failed')
end
disp('Testing foopolartocomplex...');
size=10;
a=sqrt(2)*ones(size);
b=pi/4*ones(size);
c=foopolartocomplex(a,b);
% Make sure diff is less than some small number
if (not (lt(c - complex(1,1)*ones(size), 0.000001)))
    error ('foopolartocomplex failed')
end
disp ('Success')
