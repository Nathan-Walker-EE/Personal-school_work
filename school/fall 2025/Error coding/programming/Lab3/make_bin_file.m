% random_bytes_to_binary_file.m
% Generate 1,000,448 random bytes and write as binary numbers

numBytes = 1000448;                % Total bytes
filename = 'random_bytes_bin.txt'; % Output file

% Generate random bytes (0-255)
bytes = randi([0, 255], numBytes, 1, 'uint8');

% Open file for writing
fid = fopen(filename, 'w');
if fid == -1
    error('Cannot open file for writing.');
end

% Convert each byte to an 8-bit binary string and write to file
for i = 1:numBytes
    fprintf(fid, '%s ', dec2bin(bytes(i), 8));
    if mod(i, 1024) == 0
        fprintf(fid, '\n');
    end
end

% Close file
fclose(fid);

disp(['Wrote ', num2str(numBytes), ' random bytes as binary to ', filename]);
