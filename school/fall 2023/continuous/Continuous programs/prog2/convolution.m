clear all
close all
clc


t = 0:.001:5;

f1 = [0,1,2,3,2,1];
f2 = [-2,-2,-2,-2,-2,-2,-2];
f3 = [1,-1,1,-1];
f4 = [0,0,0,-3,-3];

sol1 = conv(f1, f1);
sol2 = conv(f1, f2);
sol3 = conv(f1, f3);
sol4 = conv(f2, f3);
sol5 = conv(f1, f4);


hold on
%load convolution_1
%plot(convolution_1(:,1),convolution_1(:,2))
%load convolution_2
%plot(convolution_2(:,1),convolution_2(:,2))
%load convolution_3
%plot(convolution_3(:,1),convolution_3(:,2))
%load convolution_4
%plot(convolution_4(:,1),convolution_4(:,2))
%load convolution_5
%plot(convolution_5(:,1),convolution_5(:,2))
%plot(sol1);
%plot(sol2);
%plot(sol3);
%plot(sol4);
%plot(sol5);

h = -.3339 * exp(2.6038*-t) + .4202 * exp(-1.1981*t) .* cos(2.0787*t - deg2rad(-37.386));
f = sin(3 * pi * t);
%y = -0.1379*exp(-2.6038*t) + 0.1519*exp(-1.1981*t) .* cos(2.0797*t - .3251) + (-0.1227/(3*pi)) * sin(3*pi*t);


sol6 = conv(h, f);

%load convolution_result.txt
%plot(convolution_result(:,1),convolution_result(:,2))
plot(sol6);
%plot(t,y);

%legend('matlab', 'analytical');
%legend('matlab');

% Open a text file for writing
fileID = fopen('output.txt', 'w');

% Print each pair of (t, f(t)) to the file
for i = 1:length(t)
    fprintf(fileID, '%.6f\n', f(i));
end

% Close the file
fclose(fileID);

% Open a text file for writing
fileID = fopen('output2.txt', 'w');

% Print each pair of (t, f(t)) to the file
for i = 1:length(t)
    fprintf(fileID, '%.6f\n', h(i));
end

% Close the file
fclose(fileID);

%legend("Analytical")

hold off
ax = gca;
ax.Clipping = 'off';