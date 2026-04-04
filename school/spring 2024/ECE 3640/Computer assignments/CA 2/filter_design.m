clear 
clc

% Filter Specification
Fs = 8000; % samples/second
% Cut off frequencies
Fpass = 2000; % Hz
Fstop = 2100; % Hz
% Filter order
N = 101; % Length of filter impulse response (odd integer)

% FIR Filter Design Procedure
fpass = Fpass/Fs;
fstop = Fstop/Fs;
f1 = (fstop + fpass)/2;
f2 = (fstop - fpass)/2;
L = (N-1)/2;
n = [-L:L];
h = (sin(2*pi*f1*n) .* sin(2*pi*f2*n)) ./ ((pi*n) .* (2*pi*f2*n));
h(L+1) = 2*f1;

% Plot magnitude response
[H,F] = freqz(h,1,2^14,Fs); % Compute frequency response
subplot(211);
plot(F,abs(H),'LineWidth',2); % Plot magnitude response [linear]
xlabel('Frequency [Hertz]');
ylabel('Magnitude [linear]');
xlim([0 Fs/2]);
grid on;
subplot(212);
plot(F,20*log10(abs(H)),'LineWidth',2); % Plot magnitude response [dB]
xlabel('Frequency [Hertz]');
ylabel('Magnitude [dB]');
ylim([-80 5]); xlim([0 Fs/2]);
grid on; shg;
writematrix(h','2000Hz_lowpass.txt' );