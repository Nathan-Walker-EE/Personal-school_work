clear
clc

syms x

f = (.8-x)*(.9-x) - .1 * .2

F = [1 -1.7 .7]
roots(F)