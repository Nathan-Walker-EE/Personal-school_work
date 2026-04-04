clear 
clc

% Input signal
N = 8;
x = [1 2 3 4 5 6 7 8];


% Impulse response
L = 4;
h = [1 2 3 4];

% Convolve - call custom convolution function
y = myconv(x,h);

% Visualize
M = N+L-1; % Compute length of output
n = [0:M-1]; % Construct a time sequence
stem(n,y,'LineWidth',2); % Plot
xlabel('Time');
ylabel('Output Amplitude');
title('Output Sequence y[n]');
grid on;
shg; % Show handle graphics
print -dpng test_convolution.png

