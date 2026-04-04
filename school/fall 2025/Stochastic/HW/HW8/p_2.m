clear all; close all; clc;

w = -8*pi:0.1:8*pi;

SS = 1./(w.^2 + 4);
SN = 2./(w.^2 + 1);
H = (w.^2 + 1).  /(3*w.^2 + 9);

plot(w, SS);
hold on;
plot(w,SN);
hold off;
legend('PSD of S', 'PSD of N');
xlabel('frequency');
ylabel('Amplitude');
title('Comparison of Signal and Noise')

figure();
plot(w,H);
title('PSD of Transfer function');
xlabel('frequency');
ylabel('Amplitude');