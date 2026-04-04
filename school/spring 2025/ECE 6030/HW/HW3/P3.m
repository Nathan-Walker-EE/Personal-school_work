clear all
clc
close all

x = [1, 2, 3, 4, 5, 6];
p = [1, 2, 4, 10, 100, inf];
xp = zeros(length(p), length(x));
xps = zeros(length(p),1);
xm = xps;

for i = 1:length(p)
    xp(i,:) = x.^p(i)
    xps(i) = sum(xp(i,:))
    xm(i) = xps(i)^(1/p(i))
end
