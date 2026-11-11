function data = read_binary(filename, type)

fileID = fopen(filename, 'rb');

% Check if the file was successfully opened
if fileID == -1
    error('Error opening the file.');
end

% Read the file as double-precision floating-point numbers
data = fread(fileID, type);

% Close the file
fclose(fileID);
end

