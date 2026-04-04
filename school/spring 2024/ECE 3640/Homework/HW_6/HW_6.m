clear
clc

%% 10.12

n = 0:15;
N = 16;

xa = cos(2 * pi .* n/8);
Xa = fft(xa);
xb = sin(2 * pi .* n/8);
Xb = fft(xb);
xc = -cos(2 * pi .* n/4);
Xc = fft(xc);

figure()
subplot(2,1,1)
stem(n, abs(Xa));
title('Magnitude');
xlabel('K');
ylabel('|X|');
subplot(2,1,2);
stem(n, rad2deg(angle(Xa)));
title('Angle');
xlabel('K');
ylabel('angle(X)');

shg
print(gcf, 'P10_12a', '-dpng');

figure()
subplot(2,1,1)
stem(n, abs(Xb));
title('Magnitude');
xlabel('K');
ylabel('|X|');
subplot(2,1,2);
stem(n, rad2deg(angle(Xb)));
title('Angle');
xlabel('K');
ylabel('angle(X)');
print(gcf, 'P10_12b', '-dpng');
shg

figure()
subplot(2,1,1)
stem(n, abs(Xc));
title('Magnitude');
xlabel('K');
ylabel('|X|');
subplot(2,1,2);
stem(n, rad2deg(angle(Xc)));
title('Angle');
xlabel('K');
ylabel('angle(X)');
print(gcf, 'P10_12c', '-dpng');
shg

clear
clc
close all

%% bullet point one
n = 0:99;
Fs = 8000;

x = cos(2 * pi * 220/Fs .* n);

y = fftshift(fft(x,100));
plot((n-50)/100,abs(y));
saveas(gcf,'bullet_one_100_point','png');

n = 0:100;
y = fftshift(fft(x,101));
plot((n-50)/100,abs(y));
saveas(gcf,'bullet_one_101_point','png');

clear
clc
%% bullet point two

x = [1 1 1 1 1 1 1 1 1 1];
h = [1 1 1 1 1];

y = conv(x,h)

y2 = cconv(x,h,16)
