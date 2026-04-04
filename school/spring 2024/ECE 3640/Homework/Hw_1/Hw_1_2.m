clear 
clc

%% Variables
F = -2:.001:2;
%% Problems 

% Problem 1
x = sin(10 * pi .* F)./(pi .* F);
plot_mag(x,F,1)

%Problem 2
x = exp(-j * 10 * pi .* F) .* (sin(10 * pi .* F)./(pi .* F));
plot_mag(x,F,2)

%Problem 3
x = (sin(20 * pi .* F)./(pi .* F)) - (sin(10 * pi .* F)./(pi .* F));
plot_mag(x,F,3)

%Problem 4 
x = sincperiodic(F, 11);
plot_mag(x,F,4)

%Problem 5
x = sincperiodic(F,11) .* exp(-j*pi*F*10);
plot_mag(x,F,5)

% Problem 6
x = sincperiodic(F,21) - sincperiodic(F,9);
plot_mag(x,F,6)
