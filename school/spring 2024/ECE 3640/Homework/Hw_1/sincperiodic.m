function y=sincperiodic(x,N)
i=find(mod(x,1)==0);
x(i) = 0.5;
y = sin(pi *x * N)./sin(pi*x);
y(i) = N;
end