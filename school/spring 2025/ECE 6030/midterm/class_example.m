clear all; clc; close all;

y1 = @(t) (exp(-2*(1-t)) + 3*exp(-4*(1-t)) );
y2 = @(t) (  5/4 - 3/4*exp(-4*(1-t)) - 1/2*exp(-2*(1-t))  );

inner_product = @(f, g) integral(@(t) f(t) .* g(t), 0, 1);

I11 = inner_product(y1,y1)
I21 = inner_product(y2, y1)
I12 = I21
I22 = inner_product(y2, y2)

A = [I11, I21; I12, I22];
b = [1;0];
c = A\b

x = @(t) (c(1) * y1(t) + c(2) * y2(t));
t = 0:.01:1;
inner_product(x, y1)
inner_product(x, y2)