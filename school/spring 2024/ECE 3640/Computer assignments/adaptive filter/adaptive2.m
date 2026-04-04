clear
clc
close all

w = .01*ones(1,150);
w2 = ones(1,150);
u = .0001;
[x, Fs] = audioread("Recording.wav");
left = x(:,1);
right = x(:,2);
mono = (left + right)/2;
mono = mono.';
noise = rand(1,length(mono));
d = mono + noise;
x = noise;
xzp = zeros(1,2*length(w) + length(x) -1);
xzp(length(w):(length(x)+length(w)-1)) = x;
e = zeros(1,length(x));

tic
for n = 1:length(d)-1
    y = 0;
    for k = 1:length(w)
        y = y + w(k) * xzp(n - k + length(w));
    end
    e(n) = d(n) - y;
    for k = 1:length(w)
        w2(k) = w(k) + u * e(n) * xzp(n - k + length(w));
    end
    w = w2;
end
toc

plot(e)

