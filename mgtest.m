function mgtest
% MGTEST
%
% Test mexgen utilities

% Copyright (C) 2003-2006
% Jeffrey S. Perry
% Center for Perceptual Systems
% University of Texas at Austin
%
% jsp Wed Feb 11 14:21:15 CST 2004

save_pwd=pwd;

try
    % Are we in the same directory as this function?
    pn=which('mgtest');
    if (isempty(pn))
        error('Could not find mgtest.m');
    end
    [p,n,x,v]=fileparts(pn);

    % Build filenames
    syntaxtest_mg=fullfile(p,'syntaxtest.mg');
    abc_mg=fullfile(p,'abc.mg');
    abchandlers_cpp_save=fullfile(p,'abchandlers.cpp.save');
    xyz_mg=fullfile(p,'xyz.mg');

    % Change to an empty, temporary directory
    tmpdir=tempname;
    mkdir(tmpdir);
    cd(tmpdir);

    % Copy all of the files that we are testing here.  Otherwise, we
    % may pick up files from the mexgen toolbox directory.
    if (isunix)
        copyfile(fullfile(p,'mexgen'));
    elseif (ispc)
        copyfile(fullfile(p,'mexgen.exe'));
    else
        error('Unknown platform');
    end
    copyfile(fullfile(p,'mgcpp.m'));
    copyfile(fullfile(p,'mgentry.m'));
    copyfile(fullfile(p,'mggetpath.m'));
    copyfile(fullfile(p,'mghelp.m'));
    copyfile(fullfile(p,'mgm.m'));
    copyfile(fullfile(p,'mgmex.m'));
    copyfile(fullfile(p,'mgprompt.m'));
    copyfile(fullfile(p,'mgsyntax.m'));

    % Make sure functions are not cached
    clear functions

    % Check for read access
    check_exists(syntaxtest_mg);
    check_exists(abc_mg);
    check_exists(abchandlers_cpp_save);
    check_exists(xyz_mg);

    % Check syntax of all files
    mgsyntax(syntaxtest_mg);
    mgsyntax(abc_mg);
    mgsyntax(xyz_mg);

    % Generate abchandlers.cpp
    mgcpp(abc_mg);

    % Generate M-Files
    mgm(abc_mg);

    % Generate entry point files
    mgentry(abc_mg);

    % Copy our saved handlers file here
    copyfile(abchandlers_cpp_save,'abchandlers.cpp');

    % Generate executable
    cmd=['mex abcentry.cpp abchandlers.cpp'];
    fprintf('Evaluating %s\n',cmd);
    eval(cmd);

    % After the ABC library is built, call some of its functions to make
    % sure it works.
    disp('testing abcdonothing...');
    abcdonothing;

    % A bunch of numbers mostly in the range 0-255
    x=(randn(10,10,10)+1)*127.5;

    % Clamp them to the range 0-255
    disp('testing abcclamp...');
    x=double(abcclamp(x));

    % Make sure they are all in range
    if max(x(:))>255
        error('abcclamp failed');
    elseif min(x(:))<0
        error('abcclamp failed');
    end

    % Try a more complicated example...
    mgcpp(xyz_mg);
    mgm(xyz_mg);
    mgentry(xyz_mg);
    mex xyzentry.cpp xyzhandlers.cpp
    xyzinit
    x=[1 2 3];
    xyzfunction1(x)
    % The default xyzhandlers.cpp will not allocate any outputs, so this
    % function would fail if we called it: xyzfunction2
    xyzfunction3(x)
    u=uint32([1 2 3]);
    y=[4 5 6];
    xyzfunction4(u,y)
    % The default xyzhandlers.cpp will not allocate any outputs, so this
    % function would fail if we called it: xyzfunction5

    % Cleanup after ourselves
    % First, uncache mexfiles
    clear functions
    % Otherwise, matlab keeps those files open
    cd ..
    % Now recursively erase the entire dir
    rmdir(tmpdir,'s')

catch

    disp(lasterr)
    cd(save_pwd);
    disp('Failed');
    return

end

cd(save_pwd);
disp('Success');

function check_exists(fn)
% Throw an error if 'fn' does not exist

disp(fn)
if (not(exists(fn)))
    error('Could not find the specified file');
end

function f=exists(fn)
% Return true if 'fn' exists

fid=fopen(fn);
if fid~=-1
    fclose(fid);
    f=1;
else
    f=0;
end
