N = 50;
sigmav2 = 0.2;sigmav = sqrt(sigmav2);
v = sigmav * randn(N,1);
b = [1];
ad = [1 0.3];
d = filter(b,ad,v);

ac = [1 -0.4];
u = filter(b,ac,d);
sigman2 = 0.1; sigman = sqrt(sigman2);
n = sigman * randn(N,1);
f = n + u;

be = [0.6610 -0.2433];
dhat = filter(be,1,f);

figure(1);
n = 0:N-1;
plot(n, d);
hold on;
plot(n, dhat);
hold off;
legend('d', 'dhat', Location='best');
title('d and dhat with optimal filtering');
saveas(gcf, 'optimal_filtering.png');
esquaredopt = sum((d - dhat).^2)/N

w = -pi:.01:pi;
Hc = 1./(1 - 0.4*exp(-j.*w));
He = 0.6610 - 0.2433*exp(-j.*w);
figure(2);
plot(w,abs(Hc));
hold on;
plot(w, abs(He));
hold off;
legend('channel', 'equalizer', Location='best');
title('channel and equalizer comparison');
saveas(gcf, 'channel_equalizer.png');

be = [.4230932 -0.8945];
dhat = filter(be,1,f);

figure(3);
n = 0:N-1;
plot(n, d);
hold on;
plot(n, dhat);
hold off;
legend('d', 'dhat', Location='best');
title('d and dhat with non-optimal filtering')
saveas(gcf, 'non_optimal_filtering.png');
esquarednopt = sum((d - dhat).^2)/N