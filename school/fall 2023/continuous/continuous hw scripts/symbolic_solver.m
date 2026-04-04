clear
clc

syms t

exp = (sin(t))^2/(pi^2 * t^2);
f = int(exp, [-Inf, Inf])