clear
clc

%% problem 2
h = [1 2 3 4 4 3 2 1];
x = [1 0 0 0 0 0 0 0];
y = zeros([1 8]);

for n = 1:8
    y(n) = myFIRfilter(h,x(n));
end

n = 1:8;
figure()
subplot(2,1,1)
stem(n,h)
xlabel('n');
ylabel('h[n]');
title('impulse response');
grid on
n = 1:8;
subplot(2,1,2)
stem(n,y)
xlabel('n');
ylabel('y[n]');
title('filter response');
grid on
shg
saveas(gcf,'myFIRfilter_test','png');

%% problem 3
clear myFIRfilter
n = 0:20;
H = [.0625, .2764, .4182, .2764, .0625];
x = cos(2 * pi * 0.422 .* n);
y = zeros([1 21]);

for m = 1:21
    y(m) = myFIRfilter(H, x(m));
end

subplot(2, 1, 1)
plot(n,x);
title('prefilter');
xlabel('n')
ylabel('x[n]');
subplot(2,1,2);
plot(n, y);
title('postfilter')
xlabel('n')
ylabel('y[n]')
saveas(gcf, 'problem 3', 'png');


%% problem 4

%get impulse responses
lpf_1000 = readmatrix('1000Hz_lowpass.txt');
lpf_2000 = readmatrix('2000Hz_lowpass.txt');
lpf_1000 = lpf_1000';
lpf_2000 = lpf_2000';

% Design all pass filter
L = 50;
N = 101;
hall = zeros(size(lpf_2000));
hall(L+1) = 1; % This is the "delta"
%band stop
bsf = hall - lpf_2000 + lpf_1000;

figure()
[BSF, F] = freqz(bsf, 1, 2^14, 1);
plot(F,abs(BSF),'linewidth', 2);
xlabel('frequency');
ylabel('Magnitude [linear]')
title('Magnitude Response of Bandstop')
grid on
shg;
saveas(gcf,'my_Bandstop_filter','png');
