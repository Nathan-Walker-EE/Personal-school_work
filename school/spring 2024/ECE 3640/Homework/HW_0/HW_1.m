clear
clc

% %problem 1
% a1 = 0.9 * exp(j * 2 * pi * 0.1);
% 
% n = -10:10;
% x1 = a1.^n;
% 
% x = real(x1);
% y = imag(x1);
% 
% tiledlayout(2,2)
% 
% nexttile
% plot(x,y,".",x,y)
% title('a = 0.9exp(j 2 pi 0.1)');
% xlabel("real X[N]")
% ylabel("Imag X[N]")
% grid on
% 
% nexttile
% plot(n, x, n , y);
% title('a = 0.9exp(j 2 pi 0.1)')
% xlabel("N")
% ylabel("amplitude")
% legend("real x[n]", "imag x[n]")
% grid on
% 
% a2 = .8989 + j*0.6531;
% x2 = a2.^n;
% x = real(x2);
% y = imag(x2);
% 
% nexttile
% plot(x,y,".",x,y)
% title('a = 0.8989 + j0.6531');
% xlabel("real X[N]")
% ylabel("Imag X[N]")
% grid on
% 
% nexttile
% plot(n, x, n , y);
% title('a = 0.8989 + j0.6531');
% xlabel("N")
% ylabel("amplitude")
% legend("real x[n]", "imag x[n]")
% grid on

tiledlayout(3,2)

nexttile
t = 0:.001:10;
a = t;
plot(t,a)

nexttile
t = 0:.001:10;
a = t
plot(t,a)

nexttile
t = 0:.001:10;
a = t
plot(t,a)

nexttile
t = 0:.001:10;
a = t
plot(t,a)

 nexttile
% t = 0:.001:10;
% a = t
% plot(t,a)

nexttile
t = 0:.001:10;
a = t
plot(t,a)

% %problem 20
% n = 0:1000000;
% a = 0.9 * exp(j * 2 * pi * .1);
% 
% x = abs(a.^n).^2;
% s = sum(x)

% clear
% clc
% 
% n = -20:20;
% 
% step1 = n>=0;
% step2 = n>=4;
% 
% g = (n + 1) .* (step1 - step2);
% 
% figure
% stem(n,g);
% 
% n = -50:50;
% x = sin(pi * 2 .* n) ./(pi .* n);
% 
% %problem 18
% n = (-1:100000)';
% n = 1/(pi^2 .* n.^2);
% s18 = 2 * sum(n) + .5^2
% 
% %problem 19
% N = 10000
% n = (0:N)';
% %n = abs(sin(pi * .9 .* n)/(pi .* n))^2;
% %answer = sum(n)/N;
% 
% %problem 20
% a = 0.9 * exp(j * 2 * pi * 0.1);
% n = (0:10000)';
% %n = abs((a .^n)).^2;
% %s = sum(n)
% 
% %problem 21
% 
% n = -100000:100000;
% n = abs(cos(2 * pi * (7/20) .* n)).^2;
% %s = sum(n)


