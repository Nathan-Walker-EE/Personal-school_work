clear all
close all
clc


t = 0:.001:20;

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

a1 = 2.604;
a2 = 1.198;
b = 2.096;

h = -.3339 * exp(2.6038*-t) + .4202 * exp(-1.1981*t) .* cos(2.0787*t - deg2rad(-37.386));
f = sin(3 * pi * t);
yzs = .0112*cos(9.425 * t + 2.05) - .0329 .* exp(-2.604 * t) + .046 .* exp(-1.198 * t) .* cos(2.067 * t + .59);
yzi = -1.8356*exp(-a1 * t) - 1.1644*exp(-a2 * t) .* cos(b * t) - 1.9918*exp(-a2 * t) .* sin(b * t);

ytotal = yzs + yzi;

%plot(t,h);
%plot(t,f);
%plot(t,yzi);
%load approximation
%plot(approximation(:,1),approximation(:,2));
%plot(t,yzs);
plot(t,ytotal);
%legend('zero input', 'zero state', 'total solution');
load convolution_result.txt
plot(convolution_result(:,1),convolution_result(:,2))
%sol6 = .001 * conv(h,f);
%plot(sol6);

%legend('analytical');
%legend('matlab', 'analytical');
legend( 'analytical', 'numerical');

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

hold off
ax = gca;
ax.Clipping = 'off';