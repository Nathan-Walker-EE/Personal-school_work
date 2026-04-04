clear all;close all;clc
n = 4;
g = [1 1 1 0 1];
for i = 1:10000
    num = zeros(1,n+1);
    num(1) = 1;
    num(n+1) = 1;

    [Q, R] = deconv(num,g);
    if all(R == 0)
        disp(n)
    end
    n = n+1;
end