clear
clc

x = -10:0.1:10;
n = 4;
ycos = fcos(x,n);
plot(x,ycos),grid
xlabel('x'),ylabel('cos function')
% Define functions
function f = fcos(x,n)
         
         f = zeros(1,numel(x));
         %f = 15/2 %problem 5.9,
         %f = 9/2; %problem 5.8
         %f = 0; %problem 5.14
         %f = 4;  %proble, 5.15
         f = .25;
         w = pi;
         for i = 1:n
             %an = (40/(i^2 * pi^2)) * (cos((i*pi)/2) - cos(i * pi));%problem 5.9 
             %f = f + an*cos(i*pi*x/2); %problem 5.9
             %an = (6*(i * w* sin(i * w) - cos(i * w) + 1))/(i^2 * w^2); %problem 5.8
             %bn = (6*(-i*w*cos(i*w) + sin(i * w)))/(i^2 * w^2) ; %problem 5.8
             %an = 0;%problem 5.14
             %bn = (-10*i*w*cos(i*w) + 5*i*w*sin(i*w) + 10*i*w*cos(2*i*w) - 5*sin(i*w) - 5*sin(2*i*w) + 10*i*w)/(i^2 * w^2);%problem 5.14
             %an = (8/(i*w)) * (2*sin(2*i*w) - sin(i * w) - 2*sin(3*i*w) + sin(4*i*w));%problem 5.15
             %bn = 0;%problem 5.15
             %an = 0;
             %bn = -20/(pi * i);
             an = 1/(pi^2 * i^2) * (1 - cos(pi * i));
             bn = 1/(pi * i);
             f = f + an*cos(i * w *x) + bn*sin(i * w * x); %problem 5.8
         end
end