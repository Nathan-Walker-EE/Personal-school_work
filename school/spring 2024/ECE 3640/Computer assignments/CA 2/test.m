L = 50; N = 2*L+1; % Odd integer
n = [-L:L];
% Design LPF
f1 = 0.125; f2 = 0.025;
hlpf = (sin(2*pi*f1*n) .* sin(2*pi*f2*n)) ./ ((pi*n) .* (2*pi*f2*n));
hlpf(L+1) = 2*f1;
% Design all pass filter
hall = zeros(size(hlpf));
hall(L+1) = 1; % This is the "delta"
% Design HPF
hhpf = hall - hlpf;
% Compute frequency response
[Hhpf,F] = freqz(hhpf,1,2^14,1);

% Visualization
figure();
subplot(211);
stem(n+L,hlpf,'LineWidth',2);
xlabel('Index');
ylabel('Amplitude');
title('LPF Impulse Response')
grid on;
subplot(212);
stem(n+L,hall,'LineWidth',2);
xlabel('Index');
ylabel('Amplitude');
title('All Pass Impulse Response (delta function)')
grid on;

figure();
plot(F,abs(Hhpf),'LineWidth',2);
xlabel('Frequency');
ylabel('Magnitude [linear]');
title('Magnitude Response of High Pass Filter');
grid on;
shg;