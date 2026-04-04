clc; close all; 
r = 100/115
left = 0;right = .5;
p = 0.001;
H = @(x) (-x * log2(x)) - ((1-x)*log2(1-x));
target = 1-r;tol = .0000001;
i = 0;
while (right - left) > tol
    i = i + 1;
    mid = (left+right)/2;
    H_mid = H(mid);
    if H_mid < target
        left = mid;
    else
        right = mid;
    end
end
mid