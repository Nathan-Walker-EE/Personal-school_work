clear 
clc

N = -25:25;

h = sin(2 * pi * N/3)./(2 * pi .* N);

n = 0:1000;

x = (.5).^n;

yc = conv(h,x);

yf = zeros([1 length(x)]);

for k = 1:length(x)
    yf(k) = myFIRfilter(h,x(k));
end

yfzp = zeros(1,length(yc));
M = length(yf);
yfzp(1:M) = yf;
