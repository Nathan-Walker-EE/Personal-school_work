clear
clc
clear myIIRfilter

%% problem 5
b = [1.0082 -1.9784 0.9727];
a = [1 -1.9793, 0.9801];

[x,Fs] = audioread('rainbow_road_snip_original.wav');
y = zeros([1 length(x)]);

for n = 1:length(x)
    y(n) = myIIRfilter(b,a,x(n));
end


NFFT = 2^14;
figure();
subplot(211);
[S,F,T] = spectrogram(x,hamming(NFFT),round(0.9*NFFT),NFFT,Fs);
imagesc(T,F,20*log10(abs(S)),[0 50]);
colorbar;
set(gca,'YDir','normal');
ylim([0 1500]);
xlabel('Time [seconds]');
ylabel('Frequency [Hertz]');
title('Spectrogram of the Input x[n]');
grid on;

subplot(212);
[S,F,T] = spectrogram(y,hamming(NFFT),round(0.9*NFFT),NFFT,Fs);
imagesc(T,F,20*log10(abs(S)),[0 50]);
colorbar;
set(gca,'YDir','normal');
ylim([0 1500]);
xlabel('Time [seconds]');
ylabel('Frequency [Hertz]');
title('Spectrogram of the Output y[n]');
grid on;
shg;
orient tall;
print -dpng audio_spectrograms.png

figure()
[H_shelve, F] = freqz(b,a,Fs);
plot(F,20*log10(abs(H_shelve)),'linewidth', 2);
xlabel('frequency');
ylabel('Magnitude [dB]')
xlim([0 .3]);
title('Shelving magnitude response')
grid on
shg;
saveas(gcf,'my_shelving_filter','png');

%% Problem 6


[x,Fs] = audioread('fireflyintro.wav');
N = length(x);

q = exp(-20*pi/Fs);
Fn = 466.16;
fn = Fn/Fs;
wn = 2 * pi * fn;

b = [1, -2*cos(wn), 1];
a = [1, -2*q*cos(wn), q^2];

y = zeros([1 N]);
for n = 1:N
    y(n) = myIIRfilter(b, a, x(n));
end

NFFT = 2^14;
figure();
subplot(211);
[S,F,T] = spectrogram(x,hamming(NFFT),round(0.9*NFFT),NFFT,Fs);
imagesc(T,F,20*log10(abs(S)),[0 50]);
colorbar;
set(gca,'YDir','normal');
ylim([0 1500]);
xlabel('Time [seconds]');
ylabel('Frequency [Hertz]');
title('Spectrogram of the Input x[n]');
grid on;

subplot(212);
[S,F,T] = spectrogram(y,hamming(NFFT),round(0.9*NFFT),NFFT,Fs);
imagesc(T,F,20*log10(abs(S)),[0 50]);
colorbar;
set(gca,'YDir','normal');
ylim([0 1500]);
xlabel('Time [seconds]');
ylabel('Frequency [Hertz]');
title('Spectrogram of the Output y[n]');
grid on;
shg;
orient tall;
print -dpng audio_spectrograms2.png
