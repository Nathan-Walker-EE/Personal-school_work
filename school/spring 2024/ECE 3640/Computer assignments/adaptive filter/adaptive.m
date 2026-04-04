clear
clc
close all

% e[n] = s[n] + (h[n] - w[n]) * v[n]
% d[n] = s[n] + h[n] * v[n] equals mic 1
% y[n] = w[n] * v[n](x[n]) equals mic 2
% w[n] is attempt to filter h[n] and cut
% out the noise from mic one v[n]

% computing e[n]
% 1. y[n] = sum(w_n[k]x[n-k]) from 
% k = 0 to N -1
% 2. e[n] = d[n] - y[n]
% 3. update filter
% w_{n+1}[k] = w_n[k] + u*e[n]*x[n-k]
% k = 0,1, ...., N-1

w = .01*ones(1,256);
w2 = ones(1,256);
u = .005;
[x, Fs] = audioread("mic1.wav");
[d, Fs] = audioread("mic2.wav");
x = x.';
d = d.';
xzp = zeros(1,2*length(w) + length(x) -1);
xzp(length(w):(length(x)+length(w)-1)) = x;
e = zeros(1,length(x));

for n = 1:length(d)-1
    y = 0;
    t = 0;
    for k = 1:length(w)
        %t = t + w(k) * xzp(n - k + length(w));
        y = y + w(k) * xzp(n - k + length(w));
    end
    temp = sum(conv(w,[x(1), x(2), x(3),x(4),x(5)]))/length(w);
    e(n) = d(n) - y;
    for k = 1:length(w)
        w2(k) = w(k) + u * e(n) * xzp(n - k + length(w));
    end
    w = w2;
end

plot(e)
