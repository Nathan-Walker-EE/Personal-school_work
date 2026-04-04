clear all
clc
close all


fid = fopen('rt_error.dat', 'rb');  % Open in binary read mode
error = fread(fid,'float32'); % Adjust datatype if needed
fclose(fid);

fid = fopen('filter.dat', 'rb');  % Open in binary read mode
filter = fread(fid,'float32'); % Adjust datatype if needed
fclose(fid);

figure(1);
plot(error);
title('error');


figure(2);
plot(filter);
title("filter");

figure(3);
n = 0:length(filter) - 1;
n = n/length(filter) - .5; 
H = fftshift(fft(filter));
plot(n, abs(H));
title("fft of filter");