function mgprompt
% MGPROMPT
%
% Generate a mexgen script file by prompting the user for function
% names, inputs, outputs, handler file name, and entry point name.

% Copyright (C) 2004-2006
% Jeffrey S. Perry
% Center for Perceptual Systems
% University of Texas at Austin
% Contact: jsp@mail.utexas.edu

% jsp Mon Feb 16 15:28:51 CST 2004

% Get the filename
fprintf('Enter the filename in which to save the mexgen script.\n\n');
fprintf('Mexgen files usually end with the extension .mg.\n\n');
fprintf('For example, ''foo.mg''\n\n');
fn=input('Filename: ','s');

% If no path was specifed, make sure we are only looking in the
[p,n,x,v]=fileparts(fn);

if isempty(p)
    fn=['.' filesep fn];
end

fprintf('Checking for file %s...\n',fn);

% current directory...
% Check to see if that file already exists
fid=fopen(fn);

if fid~=-1
    fclose(fid);
    disp('That file already exists.');
    yn=input('Overwrite file? y[n] ','s');
    if isempty(yn)||yn~='y'
        error('Aborting');
    end
end

% Write the header
fid=fopen(fn,'w');

if fid==-1
    error('Could not open file for writing');
end

% Let the user put a comment in the header
comment=input('Enter a comment to place in the header: ','s');

fprintf(fid,'%% Mexgen generated file, generated on %s\n%%\n%% %s\n\n', datestr(now), comment);

% Get functions
functions=0;

while 1
    % Get the function name
    if functions==0
        fname=input('Enter the name of the first function to add: ','s');
    else
        disp('Enter the name of the next function to add,');
        fname=input('or hit RETURN to stop adding functions: ','s');
    end
    if isempty(fname)
        if functions==0
            error('No functions were entered');
        else
            break;
        end
    end
    functions=functions+1;
    % Get the inputs
    ninputs=str2double(input('Enter the number inputs that this function has: ','s'));
    inputs='';
    if ninputs>0
        % Show list of types
        fprintf('Input types may be one of:\n');
        fprintf('[1] double\n[2] complex\n[3] logical\n[4] char\n');
        fprintf('[5] single\n[6] int8\n[7] uint8\n[8] int16\n[9] uint16\n');
        fprintf('[10] int32\n[11] uint32\n[12] int64\n[13] uint64\n[14] any\n');
        fprintf('[15] varargin\n');
        % Start out empty
        inputs='(';
        for n=1:ninputs
            % Prompt user
            fprintf('Enter the number of the input type for input #%d',n);
            ntype=str2double(input(': ','s'));
            % Add it to the string
            switch ntype
                case {1}
                inputs=[inputs 'double'];
                case {2}
                inputs=[inputs 'complex'];
                case {3}
                inputs=[inputs 'logical'];
                case {4}
                inputs=[inputs 'char'];
                case {5}
                inputs=[inputs 'single'];
                case {6}
                inputs=[inputs 'int8'];
                case {7}
                inputs=[inputs 'uint8'];
                case {8}
                inputs=[inputs 'int16'];
                case {9}
                inputs=[inputs 'uint16'];
                case {10}
                inputs=[inputs 'int32'];
                case {11}
                inputs=[inputs 'uint32'];
                case {12}
                inputs=[inputs 'int64'];
                case {13}
                inputs=[inputs 'uint64'];
                case {14}
                inputs=[inputs 'any'];
                case {15}
                if n~=ninputs
                    error('''varargin'' may only be specified for the last input');
                end
                inputs=[inputs 'varargin'];
                otherwise
                error('Invalid type number');
            end
            % All but the last one gets a comma after it
            if n~=ninputs
                inputs=[inputs ','];
            end
        end
        % Match parens
        inputs=[inputs ')'];
    end
    % Get the outputs
    noutputs=str2double(input('Enter the number of outputs that this functions has: ','s'));
    outputs='';
    if noutputs>0
        % Show list of types
        fprintf('Output types may be one of:\n');
        fprintf('[1] double\n[2] complex\n[3] logical\n[4] char\n');
        fprintf('[5] single\n[6] int8\n[7] uint8\n[8] int16\n[9] uint16\n');
        fprintf('[10] int32\n[11] uint32\n[12] int64\n[13] uint64\n[14] any\n');
        fprintf('[15] varargout\n');
        % Start out empty
        outputs='[';
        for n=1:noutputs
            % Prompt user
            fprintf('Enter the number of the output type for output #%d',n);
            ntype=str2double(input(': ','s'));
            % Add it to the string
            switch ntype
                case {1}
                outputs=[outputs 'double'];
                case {2}
                outputs=[outputs 'complex'];
                case {3}
                outputs=[outputs 'logical'];
                case {4}
                outputs=[outputs 'char'];
                case {5}
                outputs=[outputs 'single'];
                case {6}
                outputs=[outputs 'int8'];
                case {7}
                outputs=[outputs 'uint8'];
                case {8}
                outputs=[outputs 'int16'];
                case {9}
                outputs=[outputs 'uint16'];
                case {10}
                outputs=[outputs 'int32'];
                case {11}
                outputs=[outputs 'uint32'];
                case {12}
                outputs=[outputs 'int64'];
                case {13}
                outputs=[outputs 'uint64'];
                case {14}
                outputs=[outputs 'any'];
                case {15}
                if n~=noutputs
                    error('''varargout'' may only be specified for the last output');
                end
                outputs=[outputs 'varargout'];
                otherwise
                error('Invalid type number');
            end
            % All but the last one gets a comma after it
            if n~=noutputs
                outputs=[outputs ','];
            end
        end
        % Match brackets
        outputs=[outputs ']'];
    end
    % Get a short comment
    comment=input('Enter a short comment to document this function: ','s');
    % Build up the function declaration
    decl='function ';
    % Add outputs
    if not(isempty(outputs))
        decl=[decl outputs '='];
    end
    % Add the name
    decl=[decl fname];
    % Add inputs
    if not(isempty(inputs))
        decl=[decl inputs];
    end
    % Write the function declaration
    fprintf(fid,'%s\n%% %s\nend\n\n',decl,comment);
end

% Get the name of the handlers file
handlers=input('Enter the name of the C++ handlers file. (e.g.: foohandlers.cpp): ','s');

if isempty(handlers)
    error('No filename was entered.  Aborting');
end

fprintf(fid,'%% Set the filename of the handlers\nhandlers(''%s'')\n\n',handlers);

% Get the name of the entry point
entrypoint=input('Enter the name of the entrypoint.\nDo NOT include a filename extension.  (e.g.: fooentry): ','s');

if isempty(entrypoint)
    error('No filename was entered.  Aborting');
end

fprintf(fid,'%% Set the entrypoint\nentrypoint(''%s'')\n',entrypoint);

fclose(fid);

fprintf('NOTE: mexgen will never overwrite your handler file.\n');
fprintf('In order to regenerate this file, it must be manually deleted.\n');
fprintf('Checking syntax of %s...\n',fn);
mgsyntax(fn);
fprintf('Generating handler file, %s...\n',handlers);
mgcpp(fn);
fprintf('Generating M-Files...\n');
mgm(fn);
fprintf('Generating entry point file...\n');
mgentry(fn);

fprintf('Would you like to edit %s in order to\nfill in the appropriate function bodies?',handlers);
yn=input(' [y]n ','s');
if isempty(yn)||yn~='n'
    edit(handlers);
    input('When you are done editing, press RETURN, and\nmexgen will compile the MEX-File...','s');
    fprintf('Generating mex file...\n');
    cmd=sprintf('mex %s.cpp %s',entrypoint,handlers); 
    eval(cmd);
    fprintf('To update your MEX-File, edit %s as appropriate and then\n',handlers);
    fprintf('rerun ''mex %s.cpp %s;'' to rebuild the MEX-File.\n\n',entrypoint,handlers);
else
    fprintf('You will need to run the following command manually\n');
    fprintf('after filling in the handlers in order to build the\n');
    fprintf('MEX-File:\n\n');
    fprintf('\tmex %s.cpp %s;\n\n',entrypoint,handlers);
end

disp('Done');
