clear
clc

  w= pi/2;
  t = -10:0.1:10;
  n = 20000;
  suma = 10;
  nvec = [-n:-1:-1,1:n];
for k = -n:n %%n could be any number, bigger n - better approximation
    if (k ~= 0)
        x = sqrt(-1) * w * k;
        A = 100;
        %cn = (-2/x^2 + (1/x^2)*(exp(2*x) + exp(-2*x)));%5.7
        %cn = 20/(x^2 * 8) * (exp(-3*x) - exp(-x) - exp(x) + exp(3*x) + x*exp(-5*x) - x*exp(3*x));%5.10
        %cn = 1/( j * 2 * 2) * (-exp(pi * (x-2))/(x-2) + exp(pi * (x + 2))/(x+2) + 1/(x-2) - 1/(x+2));
        %only works with 50V DC offset %
        %cn = (A/(12 * x^2)) * (-2 * x + 2 * x * exp(2*x) + exp(2*x) + 2 * x * exp(4 * x) - exp(4 * x));%5.11
        cn = 5/4 * (-4*exp(-4 * x)/x - exp(-4*x)/x^2 + 1/x^2);

        %cn = (A/(3 * x^2)) * (x * exp(-x) + exp(-x) - exp(x) + exp(x) * x); %5.13
        suma = suma + (cn .* exp(x * t));
    end
end
plot(t,suma)
grid