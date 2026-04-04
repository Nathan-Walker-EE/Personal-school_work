clear all; clc; close all;format compact

ISBN1 = [0 1 3 1 3 9 0 7 2 4];
ISBN2 = [0 1 3 1 9 3 0 7 2 4];
sum1 = 0; sum2 = 0;
for i = 1:length(ISBN1)
    sum1 = sum1 + (length(ISBN1) - i + 1) * ISBN1(i)
    sum2 = sum2 + (length(ISBN1) - i + 1) * ISBN2(i);
end
mod(sum1, 11)
mod(sum2, 110qq2)