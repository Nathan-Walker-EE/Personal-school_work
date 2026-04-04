clear
clc

x = -10:0.1:10;
n = 10000;
ycos = fcos(x,n);
plot(x,ycos),grid
xlabel('x'),ylabel('cos function')
% Define functions
function f = fcos(x,n)
         
         f = zeros(1,numel(x));
         f = a0;
         w = ;
         for i = 1:n
             an = ;
             bn = ;
             f = f + an*cos(i * w *x) + bn*sin(i * w * x); 
         end
end