clear
clc

n = 13;
w = 2*pi/3;
for i = 1:5

     x = j * w * i;
     A = 10;
     cn = (A/(3 * x^2)) * (x * exp(-x) + exp(-x) - exp(x) + exp(x) * x);
     %A = sqrt(an^2 + bn^2);
     %theta = atand(-bn/an);
     display(i);
     display(cn);
end