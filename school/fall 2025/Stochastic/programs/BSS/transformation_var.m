clear all;clc; close all; format compact

%% gaussian transform
mygaus = [];
for i = 1:30000
    % x = myrandn();
    mygaus = [mygaus, myrandn()];
end

histogram(mygaus, 'Normalization', 'probability');
xlabel('X=x');
ylabel('P(X=x)');
title('Gaussian from box muller');

%% exponential transform
myexp = [];
for i = 1:30000
    myexp = [myexp, my_exp()];
end

histogram(myexp, 'Normalization', 'probability');
xlabel('X=x');
ylabel('P(X=x)');
title('Gaussian from box muller');

%% sin transform
my_sin = [];
for i = 1:5000
    my_sin = [my_sin, mysin()];
end

histogram(my_sin, 'Normalization', 'probability');
xlabel('X=x');
ylabel('P(X=x)');
title('Transform from Y=sin(X)');
hold on;
t = -1:.01:1;
Y = 1./(2*pi*sqrt(1-t.^2));
plot(t,Y);
hold off;
legend('generated', 'pdf');
shg;